#pragma once

#include "TextureShaderClass.h"

class ClipplaneShaderClass : public TextureShaderClass
{
private:
	struct ClipPlane
	{
		XMFLOAT4 plane;
	};

	ID3D11Buffer* clipPlaneCBuffer;

protected:
	bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename) override;
	void ShutDownShader() override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, XMFLOAT4 plane);

public:
	ClipplaneShaderClass();
	ClipplaneShaderClass(const ClipplaneShaderClass& other);
	~ClipplaneShaderClass();

	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures, XMFLOAT4 plane);
};

