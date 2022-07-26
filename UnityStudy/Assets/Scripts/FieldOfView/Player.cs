using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public struct ViewCastInfo
{
    public bool hit;        //충돌여부
    public Vector3 point;   //ray가 도달한 위치
    public float dst;       //거리
    public float angle;     //각도

    public ViewCastInfo(bool hit, Vector3 point, float dst, float angle)
    {
        this.hit   = hit;
        this.point = point;
        this.dst   = dst;
        this.angle = angle;
    }
}

public struct Edge
{
    public Vector3 pointA, pointB;

    public Edge(Vector3 pointA, Vector3 pointB)
    {
        this.pointA = pointA;
        this.pointB = pointB;
    }
}


//전장내 아군 캐릭터
public class Player : MonoBehaviour
{
    public float view = 5.0f;  //시야
    public float speed = 8.0f; //이속
    public int stepCount = 30;
    public float stepDst = 0.0f;

    Rigidbody rigidbody;
    Vector3 velocity;
    float rotDeg;
    Camera camera;

    public LayerMask targetMask, obstalceMask;
    public List<Transform> visibleTargets = new List<Transform>();

    Mesh viewMesh;
    public MeshFilter viewMeshFilter;

    public int edgeResolveIterations;   //이진 탐색 반복 횟수
    public float edgeDstThreshold;      //중간점 Ray의 길이 임계치

    private void Start()
    {
        rigidbody = GetComponent<Rigidbody>();
        camera = Camera.main;

        viewMesh = new Mesh();
        viewMesh.name = "View Mesh";
        viewMeshFilter.mesh = viewMesh;

        StartCoroutine(FindTargetsWithDelay(0.2f));
    }

    private void Update()
    {
        //마우스 위치에 맞춰서 플레이어가 보는 방향 돌려주는 용도
        Vector3 mousePos = new Vector3(0, 1000, 0); //디버그용 
        RaycastHit hit;

        if(Physics.Raycast(camera.ScreenPointToRay(Input.mousePosition), out hit))
        {
            mousePos = hit.point;
        }

        if(mousePos.y >= 1000.0f)
        {
            Debug.Log("레이캐스트 잘못 됨");
            return;
        }

        float dz = mousePos.z - rigidbody.position.z;
        float dx = mousePos.x - rigidbody.position.x;
        rotDeg = -(Mathf.Rad2Deg * Mathf.Atan2(dz, dx) - 90);
        velocity = new Vector3(Input.GetAxisRaw("Horizontal"), 0, Input.GetAxisRaw("Vertical")).normalized * speed;

        //Debug.Log(mousePos);
        //Debug.DrawRay(transform.position, transform.forward * view, Color.green);
    }

    private void LateUpdate()
    {
        DrawFieldOfView();
    }

    private void FixedUpdate()
    {
        rigidbody.MoveRotation(Quaternion.Euler(0, rotDeg, 0));
        rigidbody.MovePosition(rigidbody.position + velocity * Time.fixedDeltaTime);

        camera.transform.position = new Vector3(rigidbody.position.x, 15, rigidbody.position.z);
    }

    IEnumerator FindTargetsWithDelay(float delay)
    {
        while(true)
        {
            yield return new WaitForSeconds(delay);
            FindVisibleTargets();
        }
    }

    void FindVisibleTargets()
    {
        visibleTargets.Clear();

        Collider[] targets = Physics.OverlapSphere(transform.position, view, targetMask);

        for(int i =0; i<targets.Length; i++)
        {
            Transform target = targets[i].transform;
            Vector3 dir = (target.position - transform.position).normalized;
            float dis = Vector3.Distance(target.position, transform.position);

            if (!Physics.Raycast(transform.position, dir, dis, obstalceMask))
            {
                visibleTargets.Add(target);

                Debug.DrawRay(transform.position, dir * dis, Color.white);
            }
        }
    }

    /// <summary>
    /// 각도에 따른 방향 구하는 함수
    /// </summary>
    /// <param name="angleDegrees">각도</param>
    /// <param name="anglelsGlobal">로컬 각인지 여부</param>
    /// <returns></returns>
    public Vector3 DirFromAngle(float angleDegrees, bool anglelsGlobal)
    {
        if (!anglelsGlobal)
            angleDegrees += transform.eulerAngles.y;

        float temp = (-angleDegrees + 90) * Mathf.Deg2Rad;
        return new Vector3(Mathf.Cos(temp), 0, Mathf.Sin(temp));
    }

    /// <summary>
    /// 특정 각도로 레이를 쏴 장애물과 충돌했는지 여부
    /// </summary>
    /// <param name="angle"></param>
    /// <returns></returns>
    ViewCastInfo ViewCast(float angle)
    {
        Vector3 dir = DirFromAngle(angle, true);
        RaycastHit hit;

        if(Physics.Raycast(transform.position, dir, out hit, view, obstalceMask))
            return new ViewCastInfo(true, hit.point, hit.distance, angle);
        else
            return new ViewCastInfo(false, transform.position + dir * view, view, angle);
    }

    Edge FindMiddleRay(ViewCastInfo min, ViewCastInfo max)
    {
        float minAngle = min.angle;
        float maxAngle = max.angle;
        Vector3 minPoint = Vector3.zero;
        Vector3 maxPoint = Vector3.zero;

        for(int i = 0; i < edgeResolveIterations; i++)
        {
            //중간점 각도
            float angle = minAngle + (maxAngle - minAngle) / 2.0f;
            ViewCastInfo temp = ViewCast(angle);
            bool edgeDstThresholdExxceed = Mathf.Abs(min.dst - temp.dst) > edgeDstThreshold;

            if(temp.hit == min.hit && !edgeDstThresholdExxceed)
            {
                minAngle = angle;
                minPoint = temp.point;
            }
            else
            {
                maxAngle = angle;
                maxPoint = temp.point;
            }
        }

        return new Edge(minPoint, maxPoint);
    }

    void DrawFieldOfView()
    {
        float stepAngleSize = 360.0f / stepCount;           //ray와 ray사이 각도
        List<Vector3> rayPoints = new List<Vector3>(10);    //찍히는 점들 저장장소
        ViewCastInfo prevViewInfo = new ViewCastInfo();     //그 전 레이 정보

        for (int i = 0; i < stepCount + 1; i++)
        {
            //float angle = transform.eulerAngles.y - 360.0f / 2 + stepAngleSize * i;
            float angle = -360.0f / 2 + stepAngleSize * i;
            ViewCastInfo viewInfo = ViewCast(angle);

            if(i != 0)
            {
                bool edgeDstThresholdExceed = Mathf.Abs(prevViewInfo.dst - viewInfo.dst) > edgeDstThreshold;

                if(prevViewInfo.hit != viewInfo.hit || (prevViewInfo.hit && viewInfo.hit && edgeDstThresholdExceed))
                {
                    Edge e = FindMiddleRay(prevViewInfo, viewInfo);

                    if(e.pointA != Vector3.zero)
                    {
                        rayPoints.Add(e.pointA);
                    }

                    if(e.pointB != Vector3.zero)
                    {
                        rayPoints.Add(e.pointB);
                    }
                }
            }

            rayPoints.Add(viewInfo.point);
            prevViewInfo = viewInfo;
        }

        //메쉬 생성에 필요한 데이터 설정
        int vertexCount = rayPoints.Count + 1;              //정점 갯수
        Vector3[] vertices = new Vector3[vertexCount];      //각 정점
        int[] triangles = new int[(vertexCount - 2) * 3];   //정점 인덱스 정보
        vertices[0] = Vector3.zero;

        for(int i = 0; i < vertexCount - 1; i++)
        {
            //vertices[i + 1] = transform.InverseTransformPoint(rayPoints[i]) - transform.InverseTransformPoint(rayPoints[i]).normalized * stepDst;
            vertices[i + 1] = transform.InverseTransformPoint(rayPoints[i]);

            if (i < vertexCount - 2)
            {
                triangles[i * 3] = 0;
                triangles[i * 3 + 1] = 1 + i;
                triangles[i * 3 + 2] = 2 + i;
            }

            //Ray확인용 디버그 선
            Debug.DrawLine(transform.position, rayPoints[i], Color.magenta);
        }

        //메쉬 생성
        viewMesh.Clear();
        viewMesh.vertices = vertices;
        viewMesh.triangles = triangles;
        viewMesh.RecalculateNormals();
    }
}
