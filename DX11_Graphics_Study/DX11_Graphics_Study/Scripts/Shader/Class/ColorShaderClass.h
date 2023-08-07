#pragma once
#include "../../DxDefine.h"
#include <fstream>

//해당 클래스를 기본 베이스 클래스로 사용하자!

class ColorShaderClass
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass& colorShader);
	~ColorShaderClass();

	virtual bool Init(ID3D11Device* device, HWND hwnd);
	virtual void ShutDown();
	virtual bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

protected:
	virtual bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename);
	virtual bool CreateVSnPS(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob);
	virtual bool CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob);
	virtual bool CreateWVPConstBuffer(ID3D11Device* device);

	virtual void ShutDownShader();
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	virtual bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};


