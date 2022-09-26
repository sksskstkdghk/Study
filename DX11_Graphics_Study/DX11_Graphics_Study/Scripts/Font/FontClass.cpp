#include "../DxDefine.h"
#include "FontClass.h"

FontClass::FontClass()
{
	font = nullptr;
	texture = nullptr;
}

FontClass::FontClass(const FontClass& other)
{
}

FontClass::~FontClass()
{
}

bool FontClass::LoadFontData(char* fontFileName)
{
	ifstream fin;
	int i;
	char temp;

	font = new FontType[95];
	if (!font)
		return false;

	fin.open(fontFileName);
	if (fin.fail())
		return false;

	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> font[i].left;
		fin >> font[i].right;
		fin >> font[i].size;
	}

	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (font)
	{
		delete[] font;
		font = nullptr;
	}
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* texFileName)
{
	bool result;

	texture = new TextureClass();
	if (!texture)
		return false;

	result = texture->Init(device, texFileName);
	if (!result)
		return false;

	return true;
}

void FontClass::ReleaseTexture()
{
	if (texture)
	{
		texture->ShutDown();
		delete texture;
		texture = nullptr;
	}
}

bool FontClass::Init(ID3D11Device* device, char* fontFileName, WCHAR* texFileName)
{
	bool result;

	result = LoadFontData(fontFileName);
	if (!result)
		return false;

	result = LoadTexture(device, texFileName);
	if (!result)
		return false;

	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType*)vertices;
	numLetters = (int)strlen(sentence);
	index = 0;

	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			//0
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].uv = XMFLOAT2(font[letter].left, 0.0f);
			index++;
			//1
			vertexPtr[index].position = XMFLOAT3(drawX + font[letter].size, drawY, 0.0f);
			vertexPtr[index].uv = XMFLOAT2(font[letter].right, 0.0f);
			index++;
			//2
			vertexPtr[index].position = XMFLOAT3(drawX + font[letter].size, drawY - 16.0f, 0.0f);
			vertexPtr[index].uv = XMFLOAT2(font[letter].right, 1.0f);
			index++;

			//0
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].uv = XMFLOAT2(font[letter].left, 0.0f);
			index++;
			//2
			vertexPtr[index].position = XMFLOAT3(drawX + font[letter].size, drawY - 16.0f, 0.0f);
			vertexPtr[index].uv = XMFLOAT2(font[letter].right, 1.0f);
			index++;
			//3
			vertexPtr[index].position = XMFLOAT3(drawX, drawY - 16.0f, 0.0f);
			vertexPtr[index].uv = XMFLOAT2(font[letter].left, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + font[letter].size + 1.0f;
		}
	}
}
