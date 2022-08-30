#pragma once

class CameraClass
{
private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	XMMATRIX viewMatrix;

public:
	CameraClass();
	CameraClass(const CameraClass& cameraClass);
	~CameraClass();

	void SetPosition(float positionX, float positionY, float positionZ);
	void SetRotation(float rotationX, float rotationY, float rotationZ);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX& viewMatrix);

};

