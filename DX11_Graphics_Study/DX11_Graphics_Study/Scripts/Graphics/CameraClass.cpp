#include "../DxDefine.h"
#include "CameraClass.h"

CameraClass::CameraClass()
{
	positionX = 0.0f;
	positionY = 0.0f;
	positionZ = 0.0f;

	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& cameraClass)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float positionX, float positionY, float positionZ)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->positionZ = positionZ;
}

void CameraClass::SetRotation(float rotationX, float rotationY, float rotationZ)
{
	this->rotationX = rotationX;
	this->rotationY = rotationY;
	this->rotationZ = rotationZ;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(positionX, positionY, positionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(rotationX, rotationY, rotationZ);
}

void CameraClass::RenderReflection(float height)
{
	XMVECTOR up, position, lookAt;
	float rad;
	XMMATRIX rotationMatrix;

	XMFLOAT3 temp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	up = XMLoadFloat3(&temp);
	//평면 반사를 위해 카메라 y값 반전
	temp = XMFLOAT3(positionX, -positionY + (height * 2.0f), positionZ);
	position = XMLoadFloat3(&temp);

	//rad = XMConvertToRadians(rotationY);	//y축회전 라디안 값으로 변경
	rad = rotationY * 0.0174532925f;
	//up = XMVector3TransformCoord(up, rotationMatrix);

	temp = XMFLOAT3(sinf(rad) + positionX, temp.y, cosf(rad) + positionZ);
	lookAt = XMLoadFloat3(&temp);

	reflectionViewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

//카메라 세팅
void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	XMFLOAT3 temp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	up = XMLoadFloat3(&temp);

	temp = XMFLOAT3(positionX, positionY, positionZ);
	position = XMLoadFloat3(&temp);

	temp = XMFLOAT3(0.0f, 0.0f, 1.0f);
	lookAt = XMLoadFloat3(&temp);

	//도(degree)->라디안(radian) 변경
	pitch = XMConvertToRadians(rotationX);
	yaw = XMConvertToRadians(rotationY);
	roll = XMConvertToRadians(rotationZ);
	
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	lookAt = position + lookAt;

	viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
}
