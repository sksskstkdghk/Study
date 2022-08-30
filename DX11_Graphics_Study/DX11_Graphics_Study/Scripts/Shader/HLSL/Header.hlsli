//정점 셰이더 바인딩 상수 버퍼
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//end VSConstBuffer

//픽셀 셰이더 바인딩 상수 버퍼
cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;    //주변 광
    float4 diffuseColor;    //빛의 색상
    float3 lightDirection;  //빛의 방향
    float padding;          //남는 잉여 데이터 크기
};

//end PSConstBuffer