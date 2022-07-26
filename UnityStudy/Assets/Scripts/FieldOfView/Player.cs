using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public struct ViewCastInfo
{
    public bool hit;        //�浹����
    public Vector3 point;   //ray�� ������ ��ġ
    public float dst;       //�Ÿ�
    public float angle;     //����

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


//���峻 �Ʊ� ĳ����
public class Player : MonoBehaviour
{
    public float view = 5.0f;  //�þ�
    public float speed = 8.0f; //�̼�
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

    public int edgeResolveIterations;   //���� Ž�� �ݺ� Ƚ��
    public float edgeDstThreshold;      //�߰��� Ray�� ���� �Ӱ�ġ

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
        //���콺 ��ġ�� ���缭 �÷��̾ ���� ���� �����ִ� �뵵
        Vector3 mousePos = new Vector3(0, 1000, 0); //����׿� 
        RaycastHit hit;

        if(Physics.Raycast(camera.ScreenPointToRay(Input.mousePosition), out hit))
        {
            mousePos = hit.point;
        }

        if(mousePos.y >= 1000.0f)
        {
            Debug.Log("����ĳ��Ʈ �߸� ��");
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
    /// ������ ���� ���� ���ϴ� �Լ�
    /// </summary>
    /// <param name="angleDegrees">����</param>
    /// <param name="anglelsGlobal">���� ������ ����</param>
    /// <returns></returns>
    public Vector3 DirFromAngle(float angleDegrees, bool anglelsGlobal)
    {
        if (!anglelsGlobal)
            angleDegrees += transform.eulerAngles.y;

        float temp = (-angleDegrees + 90) * Mathf.Deg2Rad;
        return new Vector3(Mathf.Cos(temp), 0, Mathf.Sin(temp));
    }

    /// <summary>
    /// Ư�� ������ ���̸� �� ��ֹ��� �浹�ߴ��� ����
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
            //�߰��� ����
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
        float stepAngleSize = 360.0f / stepCount;           //ray�� ray���� ����
        List<Vector3> rayPoints = new List<Vector3>(10);    //������ ���� �������
        ViewCastInfo prevViewInfo = new ViewCastInfo();     //�� �� ���� ����

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

        //�޽� ������ �ʿ��� ������ ����
        int vertexCount = rayPoints.Count + 1;              //���� ����
        Vector3[] vertices = new Vector3[vertexCount];      //�� ����
        int[] triangles = new int[(vertexCount - 2) * 3];   //���� �ε��� ����
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

            //RayȮ�ο� ����� ��
            Debug.DrawLine(transform.position, rayPoints[i], Color.magenta);
        }

        //�޽� ����
        viewMesh.Clear();
        viewMesh.vertices = vertices;
        viewMesh.triangles = triangles;
        viewMesh.RecalculateNormals();
    }
}
