#pragma once
class FrustumClass
{
private:
	float frustumValue;	//프러스텀 조절용
	XMPLANE planes[6];

public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	void ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float x, float y, float z, float radius);
	bool CheckSphere(float x, float y, float z, float radius);
	bool CheckRectangle(float x, float y, float z, float xSize, float ySize, float zSize);
};

