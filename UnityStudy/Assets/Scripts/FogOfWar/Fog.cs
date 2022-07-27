using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fog : MonoBehaviour
{
    //public Player player;
    public TileMap map;

    public Texture2D texture;

    // Start is called before the first frame update
    void Start()
    {
        texture = new Texture2D(50, 50);

        map.fogTex = texture;
        GetComponent<Terrain>().materialTemplate.mainTexture = texture;

        texture.SetPixels(map.fogColors);
        texture.Apply();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
