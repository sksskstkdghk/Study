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

	//�ùٸ��� ��� ������ �ϱ� ���� w���� �ִ� 4���� ���ͷ� ���
    input.position.w = 1.0f;
	
	//���� ��ġ�� ����, ��, �翵�� ������ ���
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    //������ ���� ���ʹ� ���� ��ǥ�迡�� ���ȴ�.
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
    
    //�ؽ��� ���ø�
    textureColor = Texture.Sample(sampleState, input.uv);
    lightDir = -lightDirection;
    //��ü�� ���� ���Ϳ� ���� ������ �����Ͽ� ���� ������ ����
    //����: ���� ����� ����
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    //���� ���� 0���� ���� ��� ������
    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
    }
    
    color = saturate(color);
    //color = color * textureColor;
    
    return color;
}