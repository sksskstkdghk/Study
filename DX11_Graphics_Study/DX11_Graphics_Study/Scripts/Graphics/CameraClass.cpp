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
