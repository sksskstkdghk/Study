#include "Header.hlsli"

//빠재의 노트: Lightshaderclass.h부터

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D Texture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    float4 worldPostion;

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
    input.position.w = 1.0f;
	
	//정점 위치를 월드, 뷰, 사영의 순으로 계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.uv = input.uv;
	
    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color;
    
    color = Texture.Sample(sampleState, input.uv);
    
    if (color.r == 0.0f || color.g == 0.0f || color.b == 0.0f)
    {
        color.a = 0.0f;
    }
    else
    {
        color.a = 1.0f;
        color = color * fontlColor;
    }
    
    return color;
}