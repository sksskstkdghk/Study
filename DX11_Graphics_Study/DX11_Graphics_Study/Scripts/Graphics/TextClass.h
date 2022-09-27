#pragma once

#include "../Font/FontClass.h"
#include "../Shader/Class/FontShaderClass.h"

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer, * indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;
	};

	FontClass* font;
	FontShaderClass* fontShader;
	int screenWidth, screenHeight;
	XMMATRIX baseViewMatrix;

	SentenceType* sentence01, * sentence02;

private:
	bool InitSentence(SentenceType** type, int maxLength, ID3D11Device* device);
	bool UpdateSentence(SentenceType* type, char* text, int posX, int posY, float red, float green, float blue, ID3D11DeviceContext* deviceContext);
	void ReleaseSentence(SentenceType** type);
	bool RenderSentencce(ID3D11DeviceContext* deviceContext, SentenceType* type, XMMATRIX world, XMMATRIX ortho);

public:
	TextClass();
	TextClass(const TextClass& other);
	~TextClass();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX baseViewMatrix);
	void ShutDOWN();
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX ortho);
	bool SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext);
};

