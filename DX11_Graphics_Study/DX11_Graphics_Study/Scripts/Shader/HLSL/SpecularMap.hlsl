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

	//�ùٸ��� ��� ������ �ϱ� ���� w���� �ִ� 4���� ���ͷ� ���
    input.position.w = 1.0f;

	//���� ��ġ�� ����, ��, �翵�� ������ ���
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
        //�ؽ��� ������ ���ݻ� ���� ���ø�
        specIntensity = Textures[2].Sample(sampleState, input.uv);
        
        //���ݻ� ���� ����(���� ���� 0���� Ŭ ����)
        //2 * ���� ���� * ����(�븻) - ���� ����
        reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);
        
        //���ݻ籤 �ѷ�
        //dot(�ݻ纤��, ������ ī�޶� ���� ����)^�ݻ� ����
        //�Ϲ������δ� �ݻ��ϴ� ���� ���� �������� ���ٸ� ����.(������̸� ������ ���� ���̱� ����)
        //specular = specularColor * pow(saturate(dot(reflection, input.ViewDirection)), specularPower);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
        
        specular = specular * specIntensity;
        color = saturate(color + specular);
    }

    return color;
}