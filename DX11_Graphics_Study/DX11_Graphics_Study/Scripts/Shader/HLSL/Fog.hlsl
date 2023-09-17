#include "Header.hlsli"

cbuffer FogBuffer : register(b2)
{
    float fogStart;
    float fogEnd;
    
    float2 fogPadding;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float fogFacter : FOG;
    
};

Texture2D Texture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    float4 camPos;

	//�ùٸ��� ��� ������ �ϱ� ���� w���� �ִ� 4���� ���ͷ� ���
    input.position.w = 1.0f;

	//���� ��ġ�� ����, ��, �翵�� ������ ���
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;

    camPos = mul(input.position, worldMatrix);
    camPos = mul(camPos, viewMatrix);
    
    //���� �Ȱ�(linear fog)
    //output.fogFacter = saturate((fogEnd - camPos.z) / (fogEnd - fogStart));
    //���� �Ȱ�(exponential fog)
    //2.71828���� �⺻ �ڿ� �α� �� �̶�� �Ѵ�(�� 2.71828�̶�� ��?)
    output.fogFacter = saturate(1.0f / (pow(2.71828, camPos.z * 0.45f)));

    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color;
    float4 textureColor, fogColor;
    
    textureColor = Texture.Sample(sampleState, input.uv);
    fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    color = input.fogFacter * textureColor + (1.0f - input.fogFacter) * fogColor;

    return color;
}