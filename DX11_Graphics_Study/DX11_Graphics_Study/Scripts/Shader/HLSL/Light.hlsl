#include "Header.hlsli"

//빠재의 노트: Lightshaderclass.h부터

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

	//올바르게 행렬 연산을 하기 위해 w까지 있는 4성분 벡터로 사용
    input.position.w = 1.0f;
	
	//정점 위치를 월드, 뷰, 사영의 순으로 계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    //정점의 법선 벡터는 월드 좌표계에서 계산된다.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);
	
    output.uv = input.uv;
    
    //카메라에서 월드 위치를 빼서 카메라를 바라보는 방향을 구한 뒤 할당
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
    //테스트용
    //specular = float4(1, 1, 1, 1);
    
    //텍스쳐 샘플링
    if (input.uv.x < 0 || input.uv.y < 0)
        textureColor = float4(1, 0, 0, 1);
    else
        textureColor = Texture.Sample(sampleState, input.uv);
    
    lightDir = -lightDirection;
    //객체의 법선 벡터와 빛의 방향을 내적하여 빛의 강도를 구함
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    //빛의 강도 값이 0보다 작은 경우 무시함
    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
        
        //정반사 벡터 구함(조도 값이 0보다 클 때만)
        //2 * 빛의 강도 * 법선(노말) - 빛의 방향
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);
        
        //정반사광 총량
        //dot(반사벡터, 정점이 카메라를 보는 방향)^반사 강도
        //일반적으로는 반사하는 빛의 색을 곱하지만 없다면 뺀다.(백색광이면 어차피 같은 값이기 때문)
        //specular = specularColor * pow(saturate(dot(reflection, input.ViewDirection)), specularPower);
        specular = pow(saturate(dot(reflection, input.ViewDirection)), specularPower);
    }
    
    //color = saturate(color);
    color = color * textureColor;
    
    //정반사 적용
    color = saturate(color + specular);
    //테스트 용
    //color = saturate(color * specular);
    
    return color;
}