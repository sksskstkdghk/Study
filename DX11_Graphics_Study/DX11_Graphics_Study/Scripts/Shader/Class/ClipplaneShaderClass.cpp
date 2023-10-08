#include "../../DxDefine.h"
#include "ClipplaneShaderClass.h"

bool ClipplaneShaderClass::InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename)
{
	bool result;
	HRESULT hResult;

	D3D11_BUFFER_DESC clipBufferDesc;

	result = __super::InitShader(device, hwnd, shaderFilename);
	if (!result)
		return false;

	//클리핑 평면 상수버퍼 생성
	clipBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	clipBufferDesc.ByteWidth = sizeof(ClipPlane);
	clipBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	clipBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	clipBufferDesc.MiscFlags = 0;
	clipBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&clipBufferDesc, nullptr, &clipPlaneCBuffer);
	if (FAILED(hResult))
		return false;

	return true;
}

void ClipplaneShaderClass::ShutDownShader()
{
	if (clipPlaneCBuffer)
	{
		clipPlaneCBuffer->Release();
		clipPlaneCBuffer = nullptr;
	}

	__super::ShutDownShader();
}

bool ClipplaneShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, XMFLOAT4 plane)
{
	HRESULT result;
	ClipPlane* dataPtr;

	if (!__super::SetShaderParameters(deviceContext, world, view, projection, textures[0]))
		return false;

	result = deviceContext->Map(clipPlaneCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	dataPtr = (ClipPlane*)mappedResource.pData;

	dataPtr->plane = plane;

	deviceContext->Unmap(clipPlaneCBuffer, 0);
	deviceContext->VSSetConstantBuffers(2, 1, &clipPlaneCBuffer);

	return true;
}

ClipplaneShaderClass::ClipplaneShaderClass()
{
	clipPlaneCBuffer = nullptr;
}

ClipplaneShaderClass::ClipplaneShaderClass(const ClipplaneShaderClass& other)
{
}

ClipplaneShaderClass::~ClipplaneShaderClass()
{
}

bool ClipplaneShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, XMFLOAT4 plane)
{
	bool result;

	result = SetShaderParameters(deviceContext, world, view, projection, textures, plane);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}
