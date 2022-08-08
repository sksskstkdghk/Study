Shader "Unlit/GaussianBlur"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}

    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            ZTest Off
            ZWrite Off
            Cull Back

            CGPROGRAM
            #pragma fragmentoption ARB _percision_hint_fastest
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
            //float4 _MainTex_ST;
            half4 _Offsets;

            static const int samplingCount = 10;
            half Weights[samplingCount];

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                half4 col = tex2D(_MainTex, i.uv);

                [loop]
                for (int j = samplingCount - 1; j > 0; j--)
                {
                    col += tex2D(_MainTex, i.uv - (_Offsets.xy * j)) * Weights[j];
                }

                [loop]
                for (int j = 0; j < samplingCount; j++)
                {
                    col += tex2D(_MainTex, i.uv + (_Offsets.xy * j)) * Weights[j];
                }
                
                return col;
            }
            ENDCG
        }
    }
}
