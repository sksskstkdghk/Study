#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass()
{
    samplerState = nullptr;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{
}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
    bool result;

    result = InitShader(device, hwnd, L"Scripts/Shader/HLSL/Texture.hlsl");
    if (!result)
        return false;

    return true;
}

void TextureShaderClass::ShutDown()
{
    ShutDownShader();
}

bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
    bool result;

    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
    if (!result)
        return false;

    RenderShader(deviceContext, indexCount);

    return true;
}

bool TextureShaderClass::InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;

	D3D11_SAMPLER_DESC samplerDesc;

	vertexShaderBlob = nullptr;
	pixelShaderBlob = nullptr;

	if (!CreateVSnPS(device, hwnd, shaderFilename, &vertexShaderBlob, &pixelShaderBlob))
		return false;

	if (!CreateLayout(device, &vertexShaderBlob, &pixelShaderBlob))
		return false;

	//����� ���� ���� ����
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

	return true;
}

bool TextureShaderClass::CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	HRESULT result;

	unsigned const int numElements = 2;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[numElements];

	//��ġ ������ ����
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	//�÷� ������ ����
	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//���̾ƿ� ����
	result = device->CreateInputLayout(polygonLayout, numElements, (*BSBlob)->GetBufferPointer(), (*BSBlob)->GetBufferSize(), &layout);
	if (FAILED(result))
	{
		cout << "���̾ƿ� ���� ����\n";
		return false;
	}

	return true;
}

void TextureShaderClass::ShutDownShader()
{
	//���÷� ������Ʈ ����
	if (samplerState)
	{
		samplerState->Release();
		samplerState = nullptr;
	}

	__super::ShutDownShader();
}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	__super::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);

	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//�Է� �ܰ迡 ���̾ƿ� ���ε�
	deviceContext->IASetInputLayout(layout);

	//����, �ȼ� ���̴� ����
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	//���÷� ���� ����
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	//indices�� �ﰢ���� �׸�
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
