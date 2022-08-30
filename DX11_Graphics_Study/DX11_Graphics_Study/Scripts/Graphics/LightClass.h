#pragma once
#include "../DxDefine.h"

class LightClass
{
private:
	XMFLOAT4 diffusetColor;
	XMFLOAT4 ambientColor;
	XMFLOAT3 direction;

public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetAmbientColor();
	XMFLOAT3 GetDirection();
};

