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

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
	input.position.w = 1.0f;

	//정점 위치를 월드, 뷰, 사영의 순으로 계산
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

	//텍스쳐01 * 텍스쳐 02 * 감마 보정 값(모니터 마다 감마 값이 다름.. 이거 어케 알아냄?? 일단 튜토에 있는 감마 값으로 보정)
	blendColor = texColor01 * texColor02 * 2.0;

	blendColor = saturate(blendColor);

	return blendColor;
}