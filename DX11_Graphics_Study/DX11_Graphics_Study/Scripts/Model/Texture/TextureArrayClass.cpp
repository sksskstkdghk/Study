#include "../../DxDefine.h"
#include "TextureArrayClass.h"

TextureArrayClass::TextureArrayClass()
{
	
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}

TextureArrayClass::~TextureArrayClass()
{
}

HRESULT TextureArrayClass::LoadTexture(WCHAR* fileName, ScratchImage* image)
{
	wstring temp(fileName);
	temp.assign(temp.end() - 3, temp.end());

	if (temp.compare(L"dds") == 0)
	{
		return LoadFromDDSFile(fileName, DDS_FLAGS_NONE, nullptr, *image);
	}
	else
	{
		return LoadFromWICFile(fileName, WIC_FLAGS_NONE, nullptr, *image);
	}

	return E_FAIL;
}

bool TextureArrayClass::AddTexture(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result;
	ScratchImage image;

	textures.push_back(nullptr);

	result = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(result))
		return false;

	//이미지 로드
	result = LoadTexture(fileName, &image);
	if (FAILED(result))
		return false;

	//셰이더 자원 변수에 이미지 로드
	result = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &textures[textures.size() - 1]);
	if (FAILED(result))
		return false;

	return true;
}

void TextureArrayClass::ShutDown()
{
	if (textures.size() > 0)
	{
		for (int i = 0; i < textures.size(); i++)
			textures[i]->Release();

		textures.clear();
		textures.shrink_to_fit();
	}
}

vector<ID3D11ShaderResourceView*> TextureArrayClass::GetTextures()
{
	return textures;
}

ID3D11ShaderResourceView* TextureArrayClass::GetTexture(int index)
{
	if (index >= textures.size())
		return nullptr;

	return textures[index];
}
