#include "../DxDefine.h"
#include "FrustumClass.h"

FrustumClass::FrustumClass()
{
	frustumValue = 0.0f;
}

FrustumClass::FrustumClass(const FrustumClass&)
{
}

FrustumClass::~FrustumClass()
{
}

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float zMin;
	float zBufferMax;	//zBuffer(depth) �ִ� �� 1.0
	XMMATRIX matrix;

	//���� ����� �þ�(near)
	zMin = -projectionMatrix.r[3].m128_f32[2] / projectionMatrix.r[2].m128_f32[2];
	//z���� �ִ� ��
	zBufferMax = screenDepth / (screenDepth - zMin);

	projectionMatrix.r[2].m128_f32[2] = zBufferMax;
	//���� �� �þ�(far)
	projectionMatrix.r[3].m128_f32[2] = -zBufferMax * zMin;

	matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

	for (int i = 0; i < 3; i++)
	{
		//Left, Bottom, Near������ ����
		XMPLANE temp = XMVectorSet
		(
			matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[i],
			matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[i],
			matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[i],
			matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[i]
		);

		temp = XMPlaneNormalize(temp);
		planes[i] = temp;

		//Right, Top, Far������ ����
		temp = XMVectorSet
		(
			matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[i],
			matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[i],
			matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[i],
			matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[i]
		);
		temp = XMPlaneNormalize(temp);
		planes[i + 3] = temp;
	}
}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	for (int i =0; i<6; i++)
	{
		//XMPlaneDotCoord�� ���ϰ��� XMVector�̶� xyzw���� ���� �� �����ϰ� �� �־� �ƹ��ų� ������ �ᵵ �ȴ�.
		//�̷��Ÿ� �׳� �� �ϳ��� �ѱ������ �� ���� XMVector����...??
		if (XMPlaneDotCoord(planes[i], XMVectorSet(x, y, z, 1.0f)).m128_f32[0] < frustumValue)
			return false;
	}

	return true;
}

//������ü ����
bool FrustumClass::CheckCube(float x, float y, float z, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - radius, y - radius, z - radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + radius, y - radius, z - radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - radius, y + radius, z - radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + radius, y + radius, z - radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - radius, y - radius, z + radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + radius, y - radius, z + radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - radius, y + radius, z + radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + radius, y + radius, z + radius, 1.0f)).m128_f32[0] < frustumValue)
			return false;
	}

	return true;
}

bool FrustumClass::CheckSphere(float x, float y, float z, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		//XMPlaneDotCoord�� ���ϰ��� XMVector�̶� xyzw���� ���� �� �����ϰ� �� �־� �ƹ��ų� ������ �ᵵ �ȴ�.
		//�̷��Ÿ� �׳� �� �ϳ��� �ѱ������ �� ���� XMVector����...??

		XMVECTOR temp = XMPlaneDotCoord(planes[i], XMVectorSet(x, y, z, 1.0f));

		if (temp.m128_f32[0] < frustumValue - radius)
			return false;
	}

	return true;
}

bool FrustumClass::CheckRectangle(float x, float y, float z, float xSize, float ySize, float zSize)
{
	for (int i = 0; i < 6; i++)
	{
		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - xSize, y - ySize, z - zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + xSize, y - ySize, z - zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - xSize, y + ySize, z - zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + xSize, y + ySize, z - zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - xSize, y - ySize, z + zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + xSize, y - ySize, z + zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x - xSize, y + ySize, z + zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;

		if (XMPlaneDotCoord(planes[i], XMVectorSet(x + xSize, y + ySize, z + zSize, 1.0f)).m128_f32[0] < frustumValue)
			return false;
	}

	return true;
}
