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

    MaterialPropertyBlock matBlock;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    private void OnDrawGizmos()
    {
        //if (visit == Visit.NONE)
        //{
        //    Gizmos.color = Color.red;
        //}
        //else if (visit == Visit.CURRENT)
        //{
        //    Gizmos.color = Color.green;
        //}
        //else if (visit == Visit.BEFORE)
        //{
        //    Gizmos.color = Color.blue;
        //}

        //Gizmos.DrawCube(transform.position, Vector3.one);

        if (visit == Visit.CURRENT)
        {
            Gizmos.color = Color.green;
            Gizmos.DrawCube(transform.position, Vector3.one);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //맨 처음 데이터 초기화 용 함수
    public void Init(MaterialPropertyBlock block)
    {
        visit = Visit.NONE;
        matBlock = block;

        RaycastHit hit;

        if(Physics.Raycast(transform.position, -Vector3.up, out hit, 10.0f, 64))
        {
            transform.position = new Vector3(transform.position.x, hit.point.y, transform.position.z);
        }
        else
        {
            transform.position = new Vector3(transform.position.x, 0, transform.position.z);
        }

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
        MeshRenderer render = GetComponent<MeshRenderer>();

        if (visit == Visit.NONE)
        {
            matBlock.SetColor("_Color", Color.red);
        }
        else if (visit == Visit.CURRENT)
        {
            matBlock.SetColor("_Color", Color.green);
        }
        else if (visit == Visit.BEFORE)
        {
            matBlock.SetColor("_Color", Color.blue);
        }

        render.SetPropertyBlock(matBlock);
    }
}
