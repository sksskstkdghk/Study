Shader "Unlit/FogShader"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Blur ("Blur", Float) = 10
    }

    SubShader
    {
        Tags { "Queue" = "Transparent+20" }
        ZTest Off // 포그 플레인이 항상 화면 앞에 그려지게 함
        Blend SrcAlpha OneMinusSrcAlpha


        /*GrabPass
        {
        }*/

        Pass
        {
            CGPROGRAM
            //CGINCLUDE
            #pragma vertex vert
            #pragma fragment frag
            
            #include "UnityCG.cginc"


            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float4 vertex : SV_POSITION;
                float2 uv : TEXCOORD0;
            };

            sampler2D _MainTex;
            //sampler2D _GrabTexture;
            //float4 _GrabTexture_TexelSize;
            float4 _MainTex_ST;
            float _Blur;

            v2f vert(appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }

            fixed4 frag(v2f i) : SV_Target
            {
                float blur = _Blur;
                blur = max(1, blur);

                fixed4 col = (0, 0, 0, 0);
                float weight_total = 0;
                float distance_normalizd = 0;
                float weight = 0;


                [loop]
                for (float x = blur - 1; x > 0; x -= 1)
                {
                    distance_normalizd = abs(x / blur);
                    weight = exp(-0.5 * pow(distance_normalizd, 2) * 5.0);
                    weight_total += weight;
                    col += tex2D(_MainTex, i.uv - (_MainTex_ST.xy * x)) * weight;
                }

                /*[loop]
                for (float y = 0; y < blur; y += 1)
                {
                    distance_normalizd = abs(y / blur);
                    weight = exp(-0.5 * pow(distance_normalizd, 2) * 5.0);
                    weight_total += weight;
                    col += tex2D(_MainTex, i.uv + (_MainTex_ST.xy * y)) * weight;
                }*/

                col = col / weight_total;
                //col = tex2D(_MainTex, i.uv);
                /*fixed4 c = fixed4(0, 0, 0, 0);
                c.a = 1.f - col.r;*/

                return col;
            }

            /*fixed4 frag(v2f i) : SV_Target
            {
                fixed4 col = tex2D(_MainTex, i.uv);

                col.a -= col.r;

                return col;
            }*/
            ENDCG
        }
    }
}
