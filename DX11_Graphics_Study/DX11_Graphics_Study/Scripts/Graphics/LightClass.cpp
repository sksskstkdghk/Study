#include "LightClass.h"

LightClass::LightClass()
{
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
