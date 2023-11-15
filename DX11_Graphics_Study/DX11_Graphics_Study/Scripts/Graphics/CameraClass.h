#pragma once

class CameraClass
{
private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	XMMATRIX viewMatrix, reflectionViewMatrix;

public:
	CameraClass();
	CameraClass(const CameraClass& cameraClass);
	~CameraClass();

	void SetPosition(float positionX, float positionY, float positionZ);
	void SetRotation(float rotationX, float rotationY, float rotationZ);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	//카메라 세팅
	void RenderReflection(float height);
	void Render();

	void GetViewMatrix(XMMATRIX& viewMatrix);
	XMMATRIX GetReflectionViewMatrix() { return reflectionViewMatrix; }

};

