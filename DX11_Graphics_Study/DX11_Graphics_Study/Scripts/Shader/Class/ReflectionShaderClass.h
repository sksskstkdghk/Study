#pragma once

#include "MultiTextureShaderClass.h"

class ReflectionShaderClass : public MultiTextureShaderClass
{
private:
	struct ReflectionType
	{
		XMMATRIX reflectMatrix;
	};

	ID3D11Buffer* reflectCBuffer;

protected:
	//bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename) override;
	bool CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob) override;
	void ShutDownShader() override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, 
		ID3D11ShaderResourceView* reflectTexture, XMMATRIX reflectMatirx);

public:
	ReflectionShaderClass();
	ReflectionShaderClass(const ReflectionShaderClass& other);
	~ReflectionShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures,
		ID3D11ShaderResourceView* reflectTexture, XMMATRIX reflectMatirx);

};


