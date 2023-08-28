#include "../../DxDefine.h"
#include "SpecMapShaderClass.h"

bool SpecMapShaderClass::InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;

	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;

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

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result))
		return false;

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffer);
	if (FAILED(result))
	{
		cout << "라이트 버퍼 생성 실패\n";
		return false;
	}

	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&cameraBufferDesc, nullptr, &cameraBuffer);
	if (FAILED(result))
	{
		cout << "카메라 버퍼 생성 실패\n";
		return false;
	}

	return true;
}

bool SpecMapShaderClass::CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	HRESULT result;

	unsigned const int numElements = 5;
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

	//노말 데이터 설정
	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//탄젠트 데이터 설정
	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	//바이노말 데이터 설정
	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	//레이아웃 생성
	result = device->CreateInputLayout(polygonLayout, numElements, (*BSBlob)->GetBufferPointer(), (*BSBlob)->GetBufferSize(), &layout);
	if (FAILED(result))
	{
		cout << "레이아웃 생성 실패\n";
		return false;
	}

	return true;
}

void SpecMapShaderClass::ShutDownShader()
{
	if (cameraBuffer)
	{
		cameraBuffer->Release();
		cameraBuffer = nullptr;
	}

	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}

	__super::ShutDownShader();
}

bool SpecMapShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures,
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT3 camPos, XMFLOAT4 specColor, float specPower)
{
	bool result;
	HRESULT hResult;

	LightBufferType* lightData;
	CameraBufferType* camData;

	result = __super::SetShaderParameters(deviceContext, world, view, projection, textures);
	if (!result)
		return false;

#pragma region 라이트 상수 버퍼 세팅
	//상수 버퍼 사용을 위해 잠굼
	hResult = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
		return false;

	//데이터 받아옴
	lightData = (LightBufferType*)mappedResource.pData;

	//데이터 할당
	lightData->ambientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightData->diffuseColor = diffuseColor;
	lightData->lightDirection = lightDirection;
	lightData->specularPower = specPower;
	lightData->specularColor = specColor;

	//다 사용했다면 잠금 해제
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);
#pragma endregion
#pragma region 카메라 상수 버퍼 세팅
	//상수 버퍼 사용을 위해 잠굼
	hResult = deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
		return false;

	//데이터 받아옴
	camData = (CameraBufferType*)mappedResource.pData;

	//데이터 할당
	camData->CameraPosition = camPos;

	//다 사용했다면 잠금 해제
	deviceContext->Unmap(cameraBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &cameraBuffer);
#pragma endregion

	return true;
}

SpecMapShaderClass::SpecMapShaderClass()
{
	lightBuffer = nullptr;
	cameraBuffer = nullptr;
}

SpecMapShaderClass::SpecMapShaderClass(const SpecMapShaderClass& other)
{

}

SpecMapShaderClass::~SpecMapShaderClass()
{
}

bool SpecMapShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures,
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT3 camPos, XMFLOAT4 specColor, float specPower)
{
	bool result;

	result = SetShaderParameters(deviceContext, world, view, projection, textures, lightDirection, diffuseColor, camPos, specColor, specPower);

	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}
