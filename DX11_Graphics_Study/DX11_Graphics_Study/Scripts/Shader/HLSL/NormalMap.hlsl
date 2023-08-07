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
    //-1.0f ~ 1.0f ���� ���� �������� ����
    //���ø��� ���� 0~1.0���� ���̶� �ش� ������ ������(�븻��) ���� ������ �� �ۿ� ���� �ʱ� ����
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    
    //���� ����
    bumpNormal = input.normal + (bumpMap.x * input.tangent) + (bumpMap.y * input.biNormal);
    bumpNormal = normalize(bumpNormal);
    
    lightDir = -simpleLightDirection;
    //���� ���� �븻 ���� ���� ������ �����ؼ� ���� ������ ����
    lightIntensity = saturate(dot(bumpNormal, lightDir));
    //���� ������ ���� ������ ����(���� ��� �̶�� ���ص� �Ǳ� �Ѵ�.)
    color = saturate(simpleDiffuseColor * lightIntensity);
    //�ؽ��� ������ ���� ������
    color = texColor * color;

    return color;
}