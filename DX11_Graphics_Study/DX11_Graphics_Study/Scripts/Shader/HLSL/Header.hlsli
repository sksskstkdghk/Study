//���� ���̴� ���ε� ��� ����
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//ī�޶� ��� ����
cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;  //ī�޶� ��ġ
    
    float padding01;          //���� �׿� ������
};


//end VSConstBuffer

//����Ʈ ��� ����
cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;    //�ֺ� ��
    float4 diffuseColor;    //���� ����
    float3 lightDirection;  //���� ����
    float specularPower;    //�ݻ� ����
    float4 specularColor;   //�ݻ��
};

cbuffer SimpleLightBuffer : register(b0)
{
    float4 simpleDiffuseColor; //���� ����
    float3 simpleLightDirection; //���� ����
    
    float padding02; //���� �׿� ������
};

//�ؽ�Ʈ �÷� ��� ����
cbuffer PixelBuffer : register(b1)
{
    float4 fontlColor;
};

//end PSConstBuffer