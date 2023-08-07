#include "Header.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

Texture2D Textures[2];
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

    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    
    output.tangent = mul(input.tangent, (float3x3) worldMatrix);
    output.tangent = normalize(output.tangent);
    
    output.biNormal = mul(input.biNormal, (float3x3) worldMatrix);
    output.biNormal = normalize(output.biNormal);

    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 texColor, bumpMap;
    float3 bumpNormal, lightDir;
    float lightIntensity;
    float4 color;

    texColor = Textures[0].Sample(sampleState, input.uv);
    
    bumpMap = Textures[1].Sample(sampleState, input.uv);
    //-1.0f ~ 1.0f 사이 값이 나오도록 변경
    //샘플링된 값이 0~1.0사이 값이라 해당 범위는 범프맵(노말맵) 연산 범위에 반 밖에 되지 않기 때문
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    
    //범프 매핑
    bumpNormal = input.normal + (bumpMap.x * input.tangent) + (bumpMap.y * input.biNormal);
    bumpNormal = normalize(bumpNormal);
    
    lightDir = -simpleLightDirection;
    //계산된 범프 노말 값에 빛의 방향을 내적해서 빛의 강도를 구함
    lightIntensity = saturate(dot(bumpNormal, lightDir));
    //빛의 강도의 빛의 색상을 곱함(빛이 백색 이라면 안해도 되긴 한다.)
    color = saturate(simpleDiffuseColor * lightIntensity);
    //텍스쳐 색상을 곱해 마무리
    color = texColor * color;

    return color;
}