#include "ReflectionShaderClass.h"

//bool ReflectionShaderClass::InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename)
//{
//	bool result;
//
//	result = InitShader(device, hwnd, shaderFilename);
//	if (!result)
//		return false;
//
//	return true;
//}

bool ReflectionShaderClass::CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	bool result;
	HRESULT hResult;
	D3D11_BUFFER_DESC reflectBufferDesc;

	result = __super::CreateLayout(device, BSBlob, PSBlob);
	if (!result)
		return false;

	reflectBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	reflectBufferDesc.ByteWidth = sizeof(ReflectionType);
	reflectBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	reflectBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	reflectBufferDesc.MiscFlags = 0;
	reflectBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&reflectBufferDesc, nullptr, &reflectCBuffer);
	if (FAILED(hResult))
		return false;

	return true;
}

void ReflectionShaderClass::ShutDownShader()
{
	if (reflectCBuffer)
	{
		reflectCBuffer->Release();
		reflectCBuffer = nullptr;
	}

	__super::ShutDownShader();
}

bool ReflectionShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, ID3D11ShaderResourceView* reflectTexture, XMMATRIX reflectMatirx)
{
	bool result;
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ReflectionType* dataPtr;

	result = __super::SetShaderParameters(deviceContext, world, view, projection, textures);
	if (result)
		return false;

	reflectMatirx = XMMatrixTranspose(reflectMatirx);

	hResult = deviceContext->Map(reflectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
		return false;

	dataPtr = (ReflectionType*)mappedResource.pData;

	dataPtr->reflectMatrix = reflectMatirx;

	deviceContext->Unmap(reflectCBuffer, 0);
	deviceContext->VSSetConstantBuffers(3, 1, &reflectCBuffer);

	deviceContext->PSSetShaderResources(1, 1, &reflectTexture);

	return true;
}

ReflectionShaderClass::ReflectionShaderClass()
{
	reflectCBuffer = nullptr;
}

ReflectionShaderClass::ReflectionShaderClass(const ReflectionShaderClass& other)
{
}

ReflectionShaderClass::~ReflectionShaderClass()
{
}

bool ReflectionShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, L"Scripts/Shader/HLSL/Reflection.hlsl");
	if (!result)
		return false;

	return true;
}

bool ReflectionShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, ID3D11ShaderResourceView* reflectTexture, XMMATRIX reflectMatirx)
{
	bool result;

	result = SetShaderParameters(deviceContext, world, view, projection, textures, reflectTexture, reflectMatirx);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}
