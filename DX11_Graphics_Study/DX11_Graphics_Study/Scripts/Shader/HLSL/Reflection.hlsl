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

	//�ùٸ��� ��� ������ �ϱ� ���� w���� �ִ� 4���� ���ͷ� ���
    input.position.w = 1.0f;

	//���� ��ġ�� ����, ��, �翵�� ������ ���
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
    
    //�ݻ� ��ġ ��ǥ�� �ؽ��� ��ǥ��� ��ȯ
    //x, -y���� w������ ������ -1 ~ 1 ���� ������ �Ǵµ� 1�� ���ϰ� 2�� ���� 0 ~ 1 ���� ������ �ٲ�
    reflectUV.x = (input.reflectionPos.x / input.reflectionPos.w + 1.0f) * 0.5f;
    reflectUV.y = (-input.reflectionPos.y / input.reflectionPos.w + 1.0f) * 0.5f;
    
    reflectColor = reflectTexture.Sample(sampleState, reflectUV);
    
    color = lerp(textureColor, reflectColor, 0.15f);

    return color;
}