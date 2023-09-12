#include "LightClass.h"

LightClass::LightClass()
{
    SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
    SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
    SetDirection(0.0f, 0.0f, 1.0f);
    SetSpecularPower(8.0f);
}

LightClass::LightClass(const LightClass& other)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
    diffusetColor = XMFLOAT4(r, g, b, a);
}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
    ambientColor = XMFLOAT4(r, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
    direction = XMFLOAT3(x, y, z);
}

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
    specularColor = XMFLOAT4(r, g, b, a);
}

void LightClass::SetSpecularPower(float power)
{
    specularPower = power;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
    return diffusetColor;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
    return ambientColor;
}

XMFLOAT3 LightClass::GetDirection()
{
    return direction;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
    return specularColor;
}

float LightClass::GetSpecularPower()
{
    return specularPower;
}
