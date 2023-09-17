#pragma once

#include "ColorShaderClass.h"

class FogShaderClass : public ColorShaderClass
{
private:
	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		XMFLOAT2 padding;
	};

	ID3D11Buffer* fogBuffer;

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* SRV, float fogStart, float fogEnd);

	bool InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename) override;
	bool CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob) override;
public:
	FogShaderClass();
	FogShaderClass(const FogShaderClass&);
	~FogShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd) override;
	void ShutDown() override;
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* SRV, float fogStart, float fogEnd);

};

