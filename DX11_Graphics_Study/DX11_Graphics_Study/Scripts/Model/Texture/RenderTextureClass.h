#pragma once

class RenderTextureClass
{
private:
	ID3D11Texture2D* RTT;
	ID3D11RenderTargetView* RTV;
	ID3D11ShaderResourceView* SRV;

public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass&);
	~RenderTextureClass();

	bool Init(ID3D11Device* device, int width, int height);
	void ShutDown();

	void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depth);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depth, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);


	ID3D11ShaderResourceView* GetSRV() { return SRV; }
};

