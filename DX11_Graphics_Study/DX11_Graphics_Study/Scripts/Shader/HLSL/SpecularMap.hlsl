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
    float3 viewDirection : TEXCOORD1;
};

Texture2D Textures[3];
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    float4 worldPos;

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
    
    worldPos = mul(input.position, worldMatrix);
    
    output.viewDirection = cameraPosition.xyz - worldPos.xyz;
    output.viewDirection = normalize(output.viewDirection);

    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 texColor, bumpMap, specIntensity, specular;
    float3 bumpNormal, lightDir, reflection;
    float lightIntensity;
    float4 color;
    
    texColor = Textures[0].Sample(sampleState, input.uv);
    
    bumpMap = Textures[1].Sample(sampleState, input.uv);
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    
    bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.biNormal;
    
    bumpNormal = normalize(bumpNormal);
    
    lightDir = -lightDirection;
    
    lightIntensity = saturate(dot(bumpNormal, lightDir));
    
    color = saturate(diffuseColor * lightIntensity);
    
    color = color * texColor;
    
    if (lightIntensity > 0.0f)
    {
        //텍스쳐 형태의 정반사 강도 샘플링
        specIntensity = Textures[2].Sample(sampleState, input.uv);
        
        //정반사 벡터 구함(조도 값이 0보다 클 때만)
        //2 * 빛의 강도 * 법선(노말) - 빛의 방향
        reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);
        
        //정반사광 총량
        //dot(반사벡터, 정점이 카메라를 보는 방향)^반사 강도
        //일반적으로는 반사하는 빛의 색을 곱하지만 없다면 뺀다.(백색광이면 어차피 같은 값이기 때문)
        //specular = specularColor * pow(saturate(dot(reflection, input.ViewDirection)), specularPower);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
        
        specular = specular * specIntensity;
        color = saturate(color + specular);
    }

    return color;
}