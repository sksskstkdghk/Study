#include "../../DxDefine.h"
#include "FogShaderClass.h"

bool FogShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* SRV, float fogStart, float fogEnd)
{
	bool result;
	HRESULT hResult;
	FogBufferType* fogData;

	result = __super::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	deviceContext->PSSetShaderResources(0, 1, &SRV);
	if (!result)
		return false;

	hResult = deviceContext->Map(fogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
		return false;

	fogData = (FogBufferType*)mappedResource.pData;

	fogData->fogStart = fogStart;
	fogData->fogEnd = fogEnd;

	deviceContext->Unmap(fogBuffer, 0);
	deviceContext->VSSetConstantBuffers(2, 1, &fogBuffer);

	return true;
}

bool FogShaderClass::InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	D3D11_BUFFER_DESC fogBufferDesc;

	vertexShaderBlob = nullptr;
	pixelShaderBlob = nullptr;

	if (!CreateVSnPS(device, hwnd, shaderFilename, &vertexShaderBlob, &pixelShaderBlob))
		return false;

	if (!CreateLayout(device, &vertexShaderBlob, &pixelShaderBlob))
		return false;

	//사용이 끝난 버퍼 해제
	vertexShaderBlob->Release();
	pixelShaderBlob->Release();

	vertexShaderBlob = nullptr;
	pixelShaderBlob = nullptr;

	if (!CreateWVPConstBuffer(device))
		return false;

	//안개 상수 버퍼 description를 작성
	fogBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	fogBufferDesc.ByteWidth = sizeof(FogBufferType);
	fogBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fogBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fogBufferDesc.MiscFlags = 0;
	fogBufferDesc.StructureByteStride = 0;

	//상수 버퍼 포인터를 생성해 해당 클래스에서 셰이더 상수 버퍼에 접근할 수 있게 한다.
	result = device->CreateBuffer(&fogBufferDesc, nullptr, &fogBuffer);
	if (FAILED(result))
	{
		cout << "포그 상수 버퍼 생성 실패\n";
		return false;
	}

	return true;
}

bool FogShaderClass::CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	HRESULT result;

	unsigned const int numElements = 2;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[numElements];

	//위치 데이터 설정
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	//컬러 데이터 설정
	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//레이아웃 생성
	result = device->CreateInputLayout(polygonLayout, numElements, (*BSBlob)->GetBufferPointer(), (*BSBlob)->GetBufferSize(), &layout);
	if (FAILED(result))
	{
		cout << "레이아웃 생성 실패\n";
		return false;
	}

	return true;
}

FogShaderClass::FogShaderClass()
{
	fogBuffer = nullptr;
}

FogShaderClass::FogShaderClass(const FogShaderClass&)
{
}

FogShaderClass::~FogShaderClass()
{
}

bool FogShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, L"Scripts/Shader/HLSL/Fog.hlsl");
	if (!result)
		return false;

	return true;
}

void FogShaderClass::ShutDown()
{
	if (fogBuffer)
	{
		fogBuffer->Release();
		fogBuffer = nullptr;
	}

	__super::ShutDown();
}

bool FogShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* SRV, float fogStart, float fogEnd)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, SRV, fogStart, fogEnd);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}
