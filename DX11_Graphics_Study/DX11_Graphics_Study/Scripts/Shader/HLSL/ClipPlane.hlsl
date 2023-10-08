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
    
    float clip : SV_ClipDistance0;
};

Texture2D Texture;
SamplerState sampleState;

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;

	//�ùٸ��� ��� ������ �ϱ� ���� w���� �ִ� 4���� ���ͷ� ���
    //��� ��� ���� ���� ���̴µ�... ���� �޶����°ɱ�?
    input.position.w = 1.0f;

	//���� ��ġ�� ����, ��, �翵�� ������ ���
    output.position = mul(input.position, worldMatrix);
    output.clip = dot(output.position, clipplane);
    
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;
    
    //output.clip = dot(mul(input.position, worldMatrix), clipplane);

    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color;
    
    color = Texture.Sample(sampleState, input.uv);
    
    return color;
}