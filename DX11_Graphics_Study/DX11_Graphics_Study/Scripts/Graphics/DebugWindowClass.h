#pragma once

class DebugWindowClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 UV;
	};


	ID3D11Buffer *vertexBuffer, *indexBuffer;
	VertexType* vertices;
	int vertexCnt, indexCnt;
	//int screenWidth, screenHeight;
	int bitMapWidth, bitMapHeight;

	XMMATRIX world;
	XMMATRIX T, R, S;

	bool InitBuffers(ID3D11Device* device);
	void ShutDownBuffers();
	void UpdateMatrix();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
public:
	DebugWindowClass();
	DebugWindowClass(const DebugWindowClass&);
	~DebugWindowClass();

	bool Init(ID3D11Device* device, int bitMapWidth, int bitMapHeight);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext);

	void SetPostion(float x, float y, float z = 0.0f);
	void SetRotation(float x, float y, float z);
	void SetWindowSize(float x, float y, float z = 0.0f);

	XMMATRIX GetWorld() { return world; }
	int GetIndexCount() { return indexCnt; }
};