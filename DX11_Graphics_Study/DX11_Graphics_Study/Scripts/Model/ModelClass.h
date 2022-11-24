#pragma once
#include "../DxDefine.h"
#include "Texture/TextureClass.h"
#include "Texture/TextureArrayClass.h"
#include <fstream>

class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		//XMFLOAT4 color;
		XMFLOAT2 uv;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
	};

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	//TextureClass* textureClass;
	TextureArrayClass* textureArrayClass;
	ModelType* model;

public:
	ModelClass();
	ModelClass(const ModelClass& modelClass);
	~ModelClass();

	bool Init(ID3D11Device* device, const char* modelDataFileName, WCHAR* fileName);
	bool AddTexture(ID3D11Device* device, WCHAR* fileName);
	void ShutDown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	vector<ID3D11ShaderResourceView*> GetTextures();

private:
	bool InitBuffers(ID3D11Device* device);
	void ShutDonwBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* fileName);
	void ReleaseTexture();

	bool LoadModel(const char* modelDataFileName);
	void ReleaseModel();
};

