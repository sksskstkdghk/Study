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

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
	input.position.w = 1.0f;
	
	//정점 위치를 월드, 뷰, 사영의 순으로 계산
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