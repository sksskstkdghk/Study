using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

public class GaussianBlur : MonoBehaviour
{
    public TileMap map;

    public Texture texture;
    public Shader shader;
    public float offset = 1.0f;
    public float blur = 100.0f;

    public Material material;
    public Renderer renderer;

    //종, 횡 각각의 블러를 적용하기 위해 버퍼처럼 사용할 랜더 텍스쳐
    public RenderTexture rt1, rt2;

    public float[] weights;
    public bool isInit = false;

    [SerializeField, Range(0, 100)]
    public int rtSize = 4;

    // Start is called before the first frame update
    void Awake()
    {
        Init();
    }

    private void OnValidate()
    {
        //if (!Application.isPlaying)
        //    return;

        
    }

    public void Update()
    {
        UpdateWeights();
        Blur();
    }

    void Init()
    {
        if (isInit)
            return;

        weights = new float[10];

        texture = new Texture2D(50, 50);

        map.fogTex = texture as Texture2D;
        //GetComponent<Terrain>().terrainData. = texture;

        //texture.filterMode = FilterMode.Point;
        //texture.wrapMode = TextureWrapMode.Clamp;

        material = new Material(shader);
        material.hideFlags = HideFlags.HideAndDontSave;

        //rt1 = new RenderTexture(texture.width / rtSize, texture.height / rtSize, 0, RenderTextureFormat.ARGB32);
        //rt2 = new RenderTexture(texture.width / rtSize, texture.height / rtSize, 0, RenderTextureFormat.ARGB32);

        rt1 = new RenderTexture(texture.width * rtSize, texture.height * rtSize, 0, RenderTextureFormat.ARGB32);
        rt2 = new RenderTexture(texture.width * rtSize, texture.height * rtSize, 0, RenderTextureFormat.ARGB32);

        rt1.name = "rt1";
        rt2.name = "rt2";

        //renderer = GetComponent<Renderer>();

        UpdateWeights();

        isInit = true;
    }

    public void Blur()
    {
        if (!isInit)
            Init();

        map.UpdateFog();

        Graphics.Blit(texture, rt1, material);

        material.SetFloatArray("Weights", weights);

        float x = offset / rt1.width;
        float y = offset / rt1.height;

        //가로 블러
        material.SetVector("_Offsets", new Vector4(x, 0, 0, 0));
        Graphics.Blit(rt1, rt2, material);

        //세로 블러
        material.SetVector("_Offsets", new Vector4(0, y, 0, 0));
        Graphics.Blit(rt2, rt1, material);

        //GetComponent<Terrain>().materialTemplate = material;
        renderer.material.mainTexture = rt1;
        GetComponent<Terrain>().materialTemplate.SetTexture("_MainTex", rt1);
    }

    void UpdateWeights()
    {
        float total = 0;
        float d = blur * blur * 0.001f;

        for(int i = 0; i < weights.Length; i++)
        {
            float x = i * 2.0f;
            float w = Mathf.Exp(-0.5f * (x * x) / d);
            weights[i] = w;

            if (i > 0)
                w *= 2.0f;

            total += w;
        }

        for(int i =0; i<weights.Length; i++)
        {
            weights[i] /= total;
        }
    }
}
