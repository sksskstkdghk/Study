#include "Header.hlsli"

cbuffer FogBuffer : register(b2)
{
    float fogStart;
    float fogEnd;
    
    float2 fogPadding;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float fogFacter : FOG;
    
};

Texture2D Texture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    float4 camPos;

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
    input.position.w = 1.0f;

	//정점 위치를 월드, 뷰, 사영의 순으로 계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;

    camPos = mul(input.position, worldMatrix);
    camPos = mul(camPos, viewMatrix);
    
    //선형 안개(linear fog)
    //output.fogFacter = saturate((fogEnd - camPos.z) / (fogEnd - fogStart));
    //지수 안개(exponential fog)
    //2.71828값은 기본 자연 로그 값 이라고 한다(약 2.71828이라는 듯?)
    output.fogFacter = saturate(1.0f / (pow(2.71828, camPos.z * 0.45f)));

    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color;
    float4 textureColor, fogColor;
    
    textureColor = Texture.Sample(sampleState, input.uv);
    fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    color = input.fogFacter * textureColor + (1.0f - input.fogFacter) * fogColor;

    return color;
}