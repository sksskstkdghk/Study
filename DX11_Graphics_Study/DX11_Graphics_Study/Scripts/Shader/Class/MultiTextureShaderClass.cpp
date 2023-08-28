#include "../../DxDefine.h"
#include "MultiTextureShaderClass.h"

bool MultiTextureShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, vector<ID3D11ShaderResourceView*> textures)
{
	ColorShaderClass::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);

	deviceContext->PSSetShaderResources(0, textures.size(), &textures[0]);

	return true;
}

MultiTextureShaderClass::MultiTextureShaderClass()
{
}

MultiTextureShaderClass::MultiTextureShaderClass(const MultiTextureShaderClass& other)
{
}

MultiTextureShaderClass::~MultiTextureShaderClass()
{
}

bool MultiTextureShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, L"Scripts/Shader/HLSL/NormalMap.hlsl");
	if (!result)
		return false;

	return true;
}

bool MultiTextureShaderClass::Init(const WCHAR* shaderFilename, ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, shaderFilename);
	if (!result)
		return false;

	return true;
}

bool MultiTextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures)
{
	bool result;

	result = SetShaderParameters(deviceContext, world, view, projection, textures);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}
