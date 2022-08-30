#include "Header.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

Texture2D Texture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
    input.position.w = 1.0f;
	
	//정점 위치를 월드, 뷰, 사영의 순으로 계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    //정점의 법선 벡터는 월드 좌표계에서 계산된다.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);
	
    output.uv = input.uv;
	
    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color, textureColor;
    float3 lightDir;
    float lightIntensity;
    
    color = ambientColor;
    
    //텍스쳐 샘플링
    textureColor = Texture.Sample(sampleState, input.uv);
    lightDir = -lightDirection;
    //객체의 법선 벡터와 빛의 방향을 내적하여 빛의 조도를 구함
    //조도: 빛의 밝기의 정도
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    //조도 값이 0보다 작은 경우 무시함
    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
    }
    
    color = saturate(color);
    //color = color * textureColor;
    
    return color;
}