#pragma once

#include <fstream>

#include "../DxDefine.h"
#include "../Model/Texture/TextureClass.h"

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;
	};

	FontType* font;
	TextureClass* texture;

private:
	bool LoadFontData(char* fontFileName);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, WCHAR* texFileName);
	void ReleaseTexture();

public:
	FontClass();
	FontClass(const FontClass& other);
	~FontClass();

	bool Init(ID3D11Device* device, char* fontFileName, WCHAR* texFileName);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

};

