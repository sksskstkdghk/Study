#include "Header.hlsli"

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
//sampler2D sampleType : register(s0);

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;

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
    
    //color = tex2D(sampleType, input.uv);
    
    return color;
}