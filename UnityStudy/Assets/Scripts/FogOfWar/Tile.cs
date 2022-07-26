using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Visit
{
    NONE = 0,   //아직 방문하지 않음
    CURRENT,    //현재 방문중임
    BEFORE,     //전에 방문했었음
}

public class Tile : MonoBehaviour
{
    public Visit visit;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //맨 처음 데이터 초기화 용 함수
    public void Init()
    {
        visit = Visit.NONE;

        Material mat = Instantiate(GetComponent<MeshRenderer>().material);
        GetComponent<MeshRenderer>().material = mat;

        SetColor();
    }

    //플레이 중 시야 계산 전 초기화
    public void PlayInit()
    {
        if(visit == Visit.CURRENT)
            visit = Visit.BEFORE;

        SetColor();
    }

    public void SetColor()
    {
        Material mat = GetComponent<MeshRenderer>().material;

        if (visit == Visit.NONE)
        {
            mat.color = Color.red;
        }
        else if (visit == Visit.CURRENT)
        {
            mat.color = Color.green;
        }
        else if (visit == Visit.BEFORE)
        {
            mat.color = Color.blue;
        }
    }
}
