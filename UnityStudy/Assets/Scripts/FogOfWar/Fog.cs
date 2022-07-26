using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fog : MonoBehaviour
{
    public Player player;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //시야 내 타일 계산
    public void ComputeView(List<List<Tile>> tiles)
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
        for (int i =(int)min.x; i < (int)max.x; i++)
        {
            for (int j = (int)min.y; j < (int)max.y; j++)
            {
                visit = new Vector2(i, j);
                target = new Vector2(x, z);

                //높이가 높은 타일은 넘어감
                if (tiles[j][i].transform.localPosition.y > player.transform.localPosition.y - 2.0f)
                    continue;

                //거리가 멀어도 넘어감
                if (Vector2.Distance(target, visit) > player.view)
                    continue;

                if(isViewTile(tiles, i, j, x, z))
                    tiles[j][i].visit = Visit.CURRENT;

                tiles[j][i].SetColor();
            }
        }
    }

    //시야 확보가 가능한 타일인가?
    public bool isViewTile(List<List<Tile>> tiles, int curX, int curZ, int playerX, int playerZ)
    {
        //플레이어가 있는 타일이면 즉시 트루 리턴
        if (curX == playerX && curZ == playerZ)
            return true;

        Debug.Log(curX + " " + curZ + " " + playerX + " " + playerZ);

        Tile current = tiles[curZ][curX];
        Tile user    = tiles[playerZ][playerX];



        #region 그 전에 해둔 것
        ////x축이 같은경우
        //if (curX == playerX)
        //{
        //    if(curZ > playerZ)
        //    {
        //        for(int i = curZ - 1; i > playerZ; i--)
        //        {
        //            if(tiles[i][curX].transform.localPosition.y > player.transform.localPosition.y - 2.0f)
        //                return false;
        //        }
        //    }
        //    else
        //    {
        //        for (int i = curZ + 1; i < playerZ; i++)
        //        {
        //            if (tiles[i][curX].transform.localPosition.y > player.transform.localPosition.y - 2.0f)
        //                return false;
        //        }
        //    }

        //    return true;
        //}
        ////z축이 같은경우
        //else if(curZ == playerZ)
        //{
        //    if (curX > playerX)
        //    {
        //        for (int i = curX - 1; i > playerX; i--)
        //        {
        //            if (tiles[curZ][i].transform.localPosition.y > player.transform.localPosition.y - 2.0f)
        //                return false;
        //        }
        //    }
        //    else
        //    {
        //        for (int i = curX + 1; i < playerX; i++)
        //        {
        //            if (tiles[curZ][i].transform.localPosition.y > player.transform.localPosition.y - 2.0f)
        //                return false;
        //        }
        //    }

        //    return true;
        //};

        //float G, AG;    //기울기, 기울기 절대값
        //int ax = 0, az = 0;//전진 거리
        //int z = playerZ - curZ;
        //int x = playerX - curX;
        //Tile temp = null;

        //G = (float)(curZ - playerZ) / (float)(curX - playerX);
        //AG = Mathf.Abs(G);

        //if(AG >= 1)
        //{
        //    while(temp != current)
        //    {
        //        if ((float)ax / (float)(az + 1) < AG)
        //            ax++;
        //        else if (((float)ax / (float)(az + 1) >= AG))
        //            az++;
        //        else
        //            return true;

        //        //if (az > Mathf.Abs(z) || ax > Mathf.Abs(x))
        //        //    return true;

        //        if (x >= 0 && z >= 0)
        //            temp = tiles[playerZ - az][playerX - ax];
        //        else if (x >= 0 && z <= 0)
        //            temp = tiles[playerZ + az][playerX - ax];
        //        else if (x <= 0 && z >= 0)
        //            temp = tiles[playerZ - az][playerX + ax];
        //        else
        //            temp = tiles[playerZ + az][playerX + ax];

        //        if (temp.transform.localPosition.y > player.transform.localPosition.y - 2.0f)
        //            return false;
        //    }
        //}
        //else
        //{
        //    while (temp != current)
        //    {
        //        if ((float)az / (float)(ax + 1) < AG)
        //            az++;
        //        else if ((float)az / (float)(ax + 1) >= AG)
        //            ax++;
        //        else
        //            return true;

        //        //if (az > Mathf.Abs(z) || ax > Mathf.Abs(x))
        //        //    return true;

        //        if (x >= 0 && z >= 0)
        //            temp = tiles[playerZ - az][playerX - ax];
        //        else if (x >= 0 && z <= 0)
        //            temp = tiles[playerZ + az][playerX - ax];
        //        else if (x <= 0 && z >= 0)
        //            temp = tiles[playerZ - az][playerX + ax];
        //        else
        //            temp = tiles[playerZ + az][playerX + ax];

        //        if (temp.transform.localPosition.y > player.transform.localPosition.y - 2.0f)
        //            return false;
        //    }
        //}
        #endregion

        return true;
    }
}
