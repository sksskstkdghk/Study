#pragma once
#include "TextureShaderClass.h"

#include <fstream>

class MultiTextureShaderClass : public TextureShaderClass
{
protected:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures);

public:
	MultiTextureShaderClass();
	MultiTextureShaderClass(const MultiTextureShaderClass& other);
	~MultiTextureShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd) override;
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, vector<ID3D11ShaderResourceView*> textures);

};

