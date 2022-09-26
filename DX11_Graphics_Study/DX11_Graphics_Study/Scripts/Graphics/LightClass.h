#pragma once
#include "../DxDefine.h"

class LightClass
{
private:
	XMFLOAT4 diffusetColor;
	XMFLOAT4 ambientColor;
	XMFLOAT3 direction;
	XMFLOAT4 specularColor;
	float specularPower;

public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetSpecularPower(float power);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetAmbientColor();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();
};

