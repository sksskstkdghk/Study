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

bool TextureClass::Init(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result;
	ScratchImage image;

	result = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(result))
		false;

	//�̹��� �ε�
	result = LoadFromWICFile(fileName, WIC_FLAGS_NONE, nullptr, image);
	if (FAILED(result))
		false;

	//���̴� �ڿ� ������ �̹��� �ε�
	result = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texture);
	if (FAILED(result))
		false;
		
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
