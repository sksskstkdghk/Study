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
    float4 reflectionPos : TEXCOORD1;
};

Texture2D Texture, reflectTexture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    matrix reflectWorld;

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
    input.position.w = 1.0f;

	//정점 위치를 월드, 뷰, 사영의 순으로 계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;
    
    reflectWorld = mul(reflectionMatrix, projectionMatrix);
    reflectWorld = mul(worldMatrix, reflectWorld);
    
    output.reflectionPos = mul(input.position, reflectWorld);

    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color, textureColor, reflectColor;
    float2 reflectUV;
    
    textureColor = Texture.Sample(sampleState, input.uv);
    
    //반사 위치 좌표를 텍스쳐 좌표계로 변환
    //x, -y값을 w값으로 나누면 -1 ~ 1 사이 값으로 되는데 1을 더하고 2로 나눠 0 ~ 1 사이 값으로 바꿈
    reflectUV.x = (input.reflectionPos.x / input.reflectionPos.w + 1.0f) * 0.5f;
    reflectUV.y = (-input.reflectionPos.y / input.reflectionPos.w + 1.0f) * 0.5f;
    
    reflectColor = reflectTexture.Sample(sampleState, reflectUV);
    
    color = lerp(textureColor, reflectColor, 0.15f);

    return color;
}