#include "Header.hlsli"

//������ ��Ʈ: Lightshaderclass.h����

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
    float3 ViewDirection : TEXCOORD1;
};

Texture2D Texture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    float4 worldPostion;

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
    
    //ī�޶󿡼� ���� ��ġ�� ���� ī�޶� �ٶ󺸴� ������ ���� �� �Ҵ�
    worldPostion = mul(input.position, worldMatrix);
    output.ViewDirection = cameraPosition.xyz - worldPostion.xyz;
    output.ViewDirection = normalize(output.ViewDirection);
	
    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color, textureColor;
    float3 lightDir;
    float lightIntensity;
    float3 reflection;
    float4 specular;
    
    color = ambientColor;
    specular = float4(0, 0, 0, 0);
    //�׽�Ʈ��
    //specular = float4(1, 1, 1, 1);
    
    //�ؽ��� ���ø�
    if (input.uv.x < 0 || input.uv.y < 0)
        textureColor = float4(1, 0, 0, 1);
    else
        textureColor = Texture.Sample(sampleState, input.uv);
    
    lightDir = -lightDirection;
    //��ü�� ���� ���Ϳ� ���� ������ �����Ͽ� ���� ������ ����
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    //���� ���� ���� 0���� ���� ��� ������
    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
        
        //���ݻ� ���� ����(���� ���� 0���� Ŭ ����)
        //2 * ���� ���� * ����(�븻) - ���� ����
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);
        
        //���ݻ籤 �ѷ�
        //dot(�ݻ纤��, ������ ī�޶� ���� ����)^�ݻ� ����
        //�Ϲ������δ� �ݻ��ϴ� ���� ���� �������� ���ٸ� ����.(������̸� ������ ���� ���̱� ����)
        //specular = specularColor * pow(saturate(dot(reflection, input.ViewDirection)), specularPower);
        specular = pow(saturate(dot(reflection, input.ViewDirection)), specularPower);
    }
    
    //color = saturate(color);
    color = color * textureColor;
    
    //���ݻ� ����
    color = saturate(color + specular);
    //�׽�Ʈ ��
    //color = saturate(color * specular);
    
    return color;
}