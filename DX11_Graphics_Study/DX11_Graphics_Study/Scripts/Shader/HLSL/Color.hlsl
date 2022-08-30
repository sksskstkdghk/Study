#include "Header.hlsli"

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

//cbuffer MatrixBuffer
//{
//    matrix worldMatrix;
//    matrix viewMatrix;
//    matrix projectionMatrix;
//};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;

	//�ùٸ��� ��� ������ �ϱ� ���� w���� �ִ� 4���� ���ͷ� ���
	input.position.w = 1.0f;
	
	//���� ��ġ�� ����, ��, �翵�� ������ ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.color = input.color;
	
	return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    return input.color;
}