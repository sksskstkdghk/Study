using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Visit
{
    NONE = 0,   //���� �湮���� ����
    CURRENT,    //���� �湮����
    BEFORE,     //���� �湮�߾���
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

    //�� ó�� ������ �ʱ�ȭ �� �Լ�
    public void Init()
    {
        visit = Visit.NONE;

        Material mat = Instantiate(GetComponent<MeshRenderer>().material);
        GetComponent<MeshRenderer>().material = mat;

        SetColor();
    }

    //�÷��� �� �þ� ��� �� �ʱ�ȭ
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
