//���� ���̴� ���ε� ��� ����
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//end VSConstBuffer

//�ȼ� ���̴� ���ε� ��� ����
cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;    //�ֺ� ��
    float4 diffuseColor;    //���� ����
    float3 lightDirection;  //���� ����
    float padding;          //���� �׿� ������ ũ��
};

//end PSConstBuffer