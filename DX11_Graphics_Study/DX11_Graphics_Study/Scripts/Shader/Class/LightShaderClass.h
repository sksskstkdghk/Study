#pragma once
#include "ColorShaderClass.h"

class LightShaderClass : public ColorShaderClass
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;	//´õ¹Ì
	};

	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass& other);
	~LightShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor);

private:
	bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFileName);
	bool CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob) override;
	void ShutDownShader();
	//void OutputShaderErrorMessage(ID3D10Blob*, HWND hwnd, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};

