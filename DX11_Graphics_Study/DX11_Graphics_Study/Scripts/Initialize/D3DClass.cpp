#include "D3DClass.h"

D3DClass::D3DClass()
{
}

D3DClass::D3DClass(const D3DClass& d3dClass)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Init(int screenWidth, int screenHeight, bool isVSTNC, HWND hwnd, bool isFullScreen, float depth, float screenNear)
{
	return false;
}

void D3DClass::ShutDown()
{
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
}

void D3DClass::EndScene()
{
}

ID3D11Device* D3DClass::GetDevice()
{
	return nullptr;
}

ID3D11DeviceContext* D3DClass::getDeviceContext()
{
	return nullptr;
}

void D3DClass::GetProjectionMatrix(D3DMATRIX& projection)
{
}

void D3DClass::GetWorldMatrix(D3DMATRIX& world)
{
}

void D3DClass::GetOrthoMatrix(D3DMATRIX& orhto)
{
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
}
