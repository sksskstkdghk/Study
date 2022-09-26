#pragma once

#include "DirectXTex.h"
#include "TextureShaderClass.h"
#include <fstream>

class FontShaderClass : public TextureShaderClass
{
private:
	struct PixelBufferType
	{
		XMFLOAT4 fontColor;
	};

	ID3D11Buffer* pixelBuffer;

private:
	bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename) override;
	void ShutDownShader() override;

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 fontColor);

public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass& other);
	~FontShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd) override;
	void ShutDown() override;
	//그대로 사용할 수 있을지도?
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 fontColor);
};

