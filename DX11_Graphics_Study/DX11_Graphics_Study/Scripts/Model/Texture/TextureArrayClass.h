#pragma once

#include "TextureClass.h"

class TextureArrayClass
{
private:
	vector<ID3D11ShaderResourceView*> textures;

	HRESULT LoadTexture(WCHAR* fileName, ScratchImage* image);
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass& other);
	~TextureArrayClass();

	bool AddTexture(ID3D11Device* device, WCHAR* fileName);
	void ShutDown();

	vector<ID3D11ShaderResourceView*> GetTextures();
	ID3D11ShaderResourceView* GetTexture(int index);
};

