#pragma once
#include "ColorShaderClass.h"

class TextureShaderClass : public ColorShaderClass
{
private:
	//struct MatrixBufferType
	//{
	//	XMMATRIX world;
	//	XMMATRIX view;
	//	XMMATRIX projection;
	//};
	//
	//ID3D11VertexShader* vertexShader;
	//ID3D11PixelShader* pixelShader;
	//ID3D11InputLayout* layout;
	//ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* samplerState;

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass& other);
	~TextureShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd) override;
	void ShutDown() override;
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

private:
	bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename) override;
	bool CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob) override;
	void ShutDownShader() override;
	//void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename) override;

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) override;
};

