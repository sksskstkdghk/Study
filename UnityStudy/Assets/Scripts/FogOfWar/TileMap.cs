using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileMap : MonoBehaviour
{
    public GameObject tilePrefab;
    public Material mat;
    public Player player;
    public List<List<Tile>> tiles;
    public Color[] fogColors;
    public Texture2D fogTex;
    public RenderTexture renderTex;

    MaterialPropertyBlock matBlock;

    float time;
    const float computeTime = 0.2f;

    const int tileWidth  = 50;
    const int tileHeight = 50;

    private void Awake()
    {
        //fog = transform.GetChild(0).GetComponent<Fog>();
        tiles = new List<List<Tile>>(tileHeight);
        fogColors = new Color[tileWidth * tileHeight];
        matBlock = new MaterialPropertyBlock();
        //fogTex = FindObjectOfType<Fog>().texture;

        for (int i = 0; i < tileHeight; i++)
        {
            tiles.Add(new List<Tile>(tileWidth));
            for (int j = 0; j < tileWidth; j++)
            {
                float height = 10.0f;

                //if (j == 25)
                //    height = 1.0f;

                GameObject tile = Instantiate(tilePrefab, transform);
                tile.transform.localPosition = new Vector3((float)j + 0.5f, height, (float)i + 0.5f);

                fogColors[i + j * tileWidth] = new Color(0, 0, 0, 1);
                tiles[i].Add(tile.GetComponent<Tile>());
                tiles[i][j].Init(matBlock);
            }
        }

        //mat.color = new Color(0.0f, 0.0f, 0.0f, 1.0f);

        time = 0.0f;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;

        if(time > computeTime)
        {
            TilesInit();
            ComputeView();

            //Graphics.Blit(fogTex, renderTex);

            time = 0.0f;
        }
    }

    public void UpdateFog()
    {
        fogTex.SetPixels(fogColors);
        fogTex.Apply();
    }

    void TilesInit()
    {
        for (int i = 0; i < tileHeight; i++)
        {
            for (int j = 0; j < tileWidth; j++)
            {
                tiles[j][i].PlayInit();
                fogColors[i + j * tileWidth] = new Color(0, 0, 0, 1);

                //if (tiles[j][i].visit == Visit.NONE)
                //    fogColors[i + j * tileWidth] = Color.black;
                //else if(tiles[j][i].visit == Visit.BEFORE)
                //    fogColors[i + j * tileWidth] = new Color(0, 0, 0, 0.8f);
            }
        }
    }

    //시야 내 타일 계산
    public void ComputeView()
    {
        int x, z;
        x = (int)player.transform.position.x;
        z = (int)player.transform.position.z;

        Vector2 min, max;
        min = new Vector2(x - (int)player.view, z - (int)player.view);
        max = new Vector2(x + (int)player.view, z + (int)player.view);

        min.x = (min.x >= 0) ? min.x : 0;
        min.y = (min.y >= 0) ? min.y : 0;
        max.x = (max.x < tileWidth) ? max.x : tileWidth - 1;
        max.y = (max.y < tileHeight) ? max.y : tileHeight - 1;

        Vector2 visit, target;
        for (int i = (int)min.x; i <= (int)max.x; i++)
        {
            for (int j = (int)min.y; j <= (int)max.y; j++)
            {
                visit = new Vector2(i, j);
                target = new Vector2(x, z);

                if (tiles[j][i].visit == Visit.CURRENT)
                    continue;

                //높이가 높은 타일은 넘어감
                if (tiles[j][i].transform.localPosition.y > player.transform.localPosition.y - 1.0f)
                    continue;

                //거리가 멀어도 넘어감
                if (Vector2.Distance(target, visit) > player.view)
                    continue;

                //단순한 레이캐스트
                //if (IsObsRayHit(i, j, x, z))
                //    continue;

                //타일 전진 알고리즘
                if (!IsViewTile(i, j, x, z))
                    continue;

                tiles[j][i].visit = Visit.CURRENT;
                fogColors[i + j * tileWidth] = new Color(0, 0, 0, 0);

                tiles[j][i].SetColor();
            }
        }
    }

    /// <summary>
    /// 시야 확보가 가능한 타일인가?
    /// </summary>
    /// <param name="destX">확인할 타일 x좌표</param>
    /// <param name="destZ">확인할 타일 z좌표</param>
    /// <param name="playerX">플레이어 타일 x좌표</param>
    /// <param name="playerZ">플레이어 타일 z좌표</param>
    /// <returns></returns>
    public bool IsViewTile(int destX, int destZ, int playerX, int playerZ)
    {
        //플레이어가 있는 타일이면 즉시 트루 리턴
        if (destX == playerX && destZ == playerZ)
            return true;

        string strLog = string.Format("목적지: x({0}), z({1})\n플레이어 위치: x({2}), z({3})", destX, destZ, playerX, playerZ);
        Debug.Log(strLog);

        int x = Mathf.Abs(destX - playerX);
        int z = Mathf.Abs(destZ - playerZ);

        float big = (x > z) ? x : z;
        float t = 1 / big;

        Vector3 dest = tiles[destZ][destX].transform.localPosition;
        Vector3 user = tiles[playerZ][playerX].transform.localPosition;

        for(int i = 1; i < big; i++)
        {
            Vector3 temp = Vector3.Lerp(user, dest, t * i);

            if(tiles[(int)temp.z][(int)temp.x].transform.localPosition.y > player.transform.localPosition.y - 1.0f)
                return false;
        }

        return true;
    }

    bool IsObsRayHit(int destX, int destZ, int playerX, int playerZ)
    {
        Vector3 user = tiles[playerZ][playerX].transform.localPosition;
        user = new Vector3(user.x, player.transform.localPosition.y, user.z);

        Vector3 dest = tiles[destZ][destX].transform.localPosition;
        dest = new Vector3(dest.x, user.y, dest.z);

        Vector3 dir = Vector3.Normalize(dest - user);
        float dis = Vector3.Distance(dest, user);

        RaycastHit hit;

        //Physics.RaycastAll()

        if (Physics.Raycast(user, dir, out hit, dis, LayerMask.GetMask("Obstacle")))
        {
            Debug.DrawLine(user, hit.point, Color.red, 0.2f);
            return true;
        }

        return false;
    }
}
