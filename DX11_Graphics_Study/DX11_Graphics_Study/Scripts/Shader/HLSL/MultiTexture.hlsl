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

	return output;
}

float4 PS(PixelInputType input) : SV_Target
{
	float4 texColor01, texColor02;
	float4 blendColor;

	texColor01 = Textures[0].Sample(sampleState, input.uv);
	texColor02 = Textures[1].Sample(sampleState, input.uv);

	//�ؽ���01 * �ؽ��� 02 * ���� ���� ��(����� ���� ���� ���� �ٸ�.. �̰� ���� �˾Ƴ�?? �ϴ� Ʃ�信 �ִ� ���� ������ ����)
	blendColor = texColor01 * texColor02 * 2.0;

	blendColor = saturate(blendColor);

	return blendColor;
}