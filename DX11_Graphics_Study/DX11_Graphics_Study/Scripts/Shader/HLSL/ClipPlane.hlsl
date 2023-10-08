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
    
    float clip : SV_ClipDistance0;
};

Texture2D Texture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
    //사실 없어도 차이 없어 보이는데... 뭐가 달라지는걸까?
    input.position.w = 1.0f;

	//정점 위치를 월드, 뷰, 사영의 순으로 계산
    output.position = mul(input.position, worldMatrix);
    output.clip = dot(output.position, clipplane);
    
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;
    
    //output.clip = dot(mul(input.position, worldMatrix), clipplane);

    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color;
    
    color = Texture.Sample(sampleState, input.uv);
    
    return color;
}