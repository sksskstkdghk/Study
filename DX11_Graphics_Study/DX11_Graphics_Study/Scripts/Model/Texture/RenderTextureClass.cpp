#include "../../DxDefine.h"
#include "RenderTextureClass.h"

RenderTextureClass::RenderTextureClass()
{
	RTT = nullptr;
	RTV = nullptr;
	SRV = nullptr;
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass&)
{
}

RenderTextureClass::~RenderTextureClass()
{
}

bool RenderTextureClass::Init(ID3D11Device* device, int width, int height)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

#pragma region ���� �ؽ��� �ʱ�ȭ
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDesc, nullptr, &RTT);
	if (FAILED(result))
		return false;
#pragma endregion

#pragma region ���� Ÿ�� �� �ʱ�ȭ
	RTVDesc.Format = textureDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	result = device->CreateRenderTargetView(RTT, &RTVDesc, &RTV);
	if (FAILED(result))
		return false;
#pragma endregion

#pragma region ���̴� ���ҽ� �� �ʱ�ȭ
	SRVDesc.Format = textureDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(RTT, &SRVDesc, &SRV);
	if (FAILED(result))
		return false;
#pragma endregion

	return true;
}

void RenderTextureClass::ShutDown()
{
	if (SRV)
	{
		SRV->Release();
		SRV = nullptr;
	}

	if (RTV)
	{
		RTV->Release();
		RTV = nullptr;
	}

	if (RTT)
	{
		RTT->Release();
		RTT = nullptr;
	}
}

void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depth)
{
	deviceContext->OMSetRenderTargets(1, &RTV, depth);
}

//���� ����� �ʱ�ȭ �ϵ��� �ش� ���� Ÿ�ٿ� ���� ��� �׸��� �� ���� Ÿ�� �ʱ�ȭ�ϴ� �Լ�
void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depth, float r, float g, float b, float a)
{
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	deviceContext->ClearRenderTargetView(RTV, color);

	deviceContext->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
