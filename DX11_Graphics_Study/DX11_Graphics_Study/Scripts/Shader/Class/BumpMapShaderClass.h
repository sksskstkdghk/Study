#pragma once

#include "MultiTextureShaderClass.h"

class BumpMapShaderClass : public MultiTextureShaderClass
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

	ID3D11Buffer* lightBuffer;

protected:
	bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename) override;
	bool CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob) override;
	void ShutDownShader() override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor);

public:
	BumpMapShaderClass();
	BumpMapShaderClass(const BumpMapShaderClass& other);
	~BumpMapShaderClass();

	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor);

};

