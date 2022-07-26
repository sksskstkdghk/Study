using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileMap : MonoBehaviour
{
    public GameObject tilePrefab;
    public Material mat;
    public Player player;
    public List<List<Tile>> tiles;
    //public Fog fog;

    float time;
    const float computeTime = 0.2f;

    // Start is called before the first frame update
    void Start()
    {
        //fog = transform.GetChild(0).GetComponent<Fog>();
        tiles = new List<List<Tile>>(50);

        for (int i =0; i<50; i++)
        {
            tiles.Add(new List<Tile>(50));
            for (int j = 0; j < 50; j++)
            {
                float height = -0.4f;

                //if (j == 25)
                //    height = 1.0f;

                GameObject tile = Instantiate(tilePrefab, transform);
                tile.transform.localPosition = new Vector3((float)j + 0.5f, height, (float)i + 0.5f);

                tiles[i].Add(tile.GetComponent<Tile>());
                tiles[i][j].Init();
            }
        }

        mat.color = new Color(0.0f, 0.0f, 0.0f, 1.0f);

        time = 0.0f;
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;

        if(time > computeTime)
        {
            TilesInit();
            ComputeView();
            time = 0.0f;
        }
    }

    void TilesInit()
    {
        for (int i = 0; i < 50; i++)
        {
            for (int j = 0; j < 50; j++)
            {
                tiles[i][j].PlayInit();
            }
        }
    }

    //�þ� �� Ÿ�� ���
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
        max.x = (max.x < 50) ? max.x : 50;
        max.y = (max.y < 50) ? max.y : 50;

        Vector2 visit, target;
        for (int i = (int)min.x; i < (int)max.x; i++)
        {
            for (int j = (int)min.y; j < (int)max.y; j++)
            {
                visit = new Vector2(i, j);
                target = new Vector2(x, z);

                //���̰� ���� Ÿ���� �Ѿ
                if (tiles[j][i].transform.localPosition.y > player.transform.localPosition.y - 1.0f)
                    continue;

                //�Ÿ��� �־ �Ѿ
                if (Vector2.Distance(target, visit) > player.view)
                    continue;

                //���� ������ ��ֹ��� ������ �Ѿ
                if (IsObsRayHit(i, j, x, z))
                    continue;

                if (IsViewTile(i, j, x, z))
                    tiles[j][i].visit = Visit.CURRENT;

                tiles[j][i].SetColor();
            }
        }
    }

    /// <summary>
    /// �þ� Ȯ���� ������ Ÿ���ΰ�?
    /// </summary>
    /// <param name="destX">Ȯ���� Ÿ�� x��ǥ</param>
    /// <param name="destZ">Ȯ���� Ÿ�� z��ǥ</param>
    /// <param name="playerX">�÷��̾� Ÿ�� x��ǥ</param>
    /// <param name="playerZ">�÷��̾� Ÿ�� z��ǥ</param>
    /// <returns></returns>
    public bool IsViewTile(int destX, int destZ, int playerX, int playerZ)
    {
        //�÷��̾ �ִ� Ÿ���̸� ��� Ʈ�� ����
        if (destX == playerX && destZ == playerZ)
            return true;

        string strLog = string.Format("������: x({0}), z({1})\n�÷��̾� ��ġ: x({2}), z({3})", destX, destZ, playerX, playerZ);
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

        Physics.RaycastAll()

        if (Physics.Raycast(user, dir, out hit, dis, LayerMask.GetMask("Obstacle")))
        {
            Debug.DrawLine(user, hit.point, Color.red, 0.2f);
            return true;
        }

        return false;
    }
}
