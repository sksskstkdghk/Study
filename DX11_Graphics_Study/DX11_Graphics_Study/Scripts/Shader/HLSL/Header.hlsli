//정점 셰이더 바인딩 상수 버퍼
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//카메라 상수 버퍼
cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;  //카메라 위치
    
    float padding01;          //남는 잉여 데이터
};


//end VSConstBuffer

//라이트 상수 버퍼
cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;    //주변 광
    float4 diffuseColor;    //빛의 색상
    float3 lightDirection;  //빛의 방향
    float specularPower;    //반사 강도
    float4 specularColor;   //반사색
};

cbuffer SimpleLightBuffer : register(b0)
{
    float4 simpleDiffuseColor; //빛의 색상
    float3 simpleLightDirection; //빛의 방향
    
    float padding02; //남는 잉여 데이터
};

//텍스트 컬러 상수 버퍼
cbuffer PixelBuffer : register(b1)
{
    float4 fontlColor;
};

//end PSConstBuffer