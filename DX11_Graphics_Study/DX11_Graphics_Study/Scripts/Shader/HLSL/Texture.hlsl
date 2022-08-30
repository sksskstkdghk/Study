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
};

Texture2D Texture;
SamplerState sampleState;
//sampler2D sampleType : register(s0);

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
	
    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    float4 color;
    
    color = Texture.Sample(sampleState, input.uv);
    
    //color = tex2D(sampleType, input.uv);
    
    return color;
}