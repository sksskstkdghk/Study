#pragma once

#include "DirectXTex.h"

class TextureClass
{
private:
	ID3D11ShaderResourceView* texture;

public:
	TextureClass();
	TextureClass(const TextureClass& textureClass);
	~TextureClass();

	bool Init(ID3D11Device* device, WCHAR* fileName);
	void ShutDown();

	ID3D11ShaderResourceView* GetTexture();
};

