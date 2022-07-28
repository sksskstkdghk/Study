using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public float view = 5.0f;  //시야
    public float speed = 8.0f; //이속

    Rigidbody rigidbody;
    Vector3 velocity;
    Camera camera;

    // Start is called before the first frame update
    void Start()
    {
        rigidbody = GetComponent<Rigidbody>();
        camera = Camera.main;
    }

    // Update is called once per frame
    void Update()
    {
        //tex.SetPixels(map.fogColors);
        //tex.wrapMode = TextureWrapMode.Clamp;
        //tex.anisoLevel = 10;

        velocity = new Vector3(Input.GetAxisRaw("Horizontal"), 0, Input.GetAxisRaw("Vertical")).normalized * speed;
    }

    private void FixedUpdate()
    {
        //rigidbody.MoveRotation(Quaternion.Euler(0, rotDeg, 0));
        rigidbody.MovePosition(rigidbody.position + velocity * Time.fixedDeltaTime);

        camera.transform.position = new Vector3(rigidbody.position.x, 15, rigidbody.position.z - 5);
    }
}
