#include "../../DxDefine.h"
#include "TextureClass.h"

TextureClass::TextureClass()
{
	texture = nullptr;
}

TextureClass::TextureClass(const TextureClass& textureClass)
{
}

TextureClass::~TextureClass()
{
}

HRESULT TextureClass::LoadTexture(WCHAR* fileName, ScratchImage* image)
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


bool TextureClass::Init(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result;
	ScratchImage image;

	result = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(result))
		return false;

	//이미지 로드
	result = LoadTexture(fileName, &image);
	if (FAILED(result))
		return false;

	//셰이더 자원 변수에 이미지 로드
	result = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texture);
	if (FAILED(result))
		return false;
		
	return true;
}

void TextureClass::ShutDown()
{
	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return texture;
}
