#pragma once

#include "../Model/Texture/TextureClass.h"

class BitmapClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;
	};

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	TextureClass* texture;

	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int prevPosX, prevPosY;

private:
	bool InitBuffer(ID3D11Device* device);
	void ShutDownBuffer();
	bool UpdateBuffer(ID3D11DeviceContext* deviceContext, int posX, int posY);
	void RenderBuffer(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* fileName);
	void ReleaseTexture();

public:
	BitmapClass();
	BitmapClass(const BitmapClass& bitmap);
	~BitmapClass();

	bool Init(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFileName, int bitmapWidth, int bitmapHeight);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, int posX, int posY);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
};

