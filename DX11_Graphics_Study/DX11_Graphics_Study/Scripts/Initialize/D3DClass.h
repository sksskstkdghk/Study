#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3d9types.h>

class D3DClass
{
private:
	bool isVsync;
	int videoCardMemory;
	char videoCardDescription[128];

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D10DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	D3DMATRIX projectionMatrix;
	D3DMATRIX worldMatrix;
	D3DMATRIX orthoMatrix;

public:
	D3DClass();
	D3DClass(const D3DClass& d3dClass);
	~D3DClass();

	bool Init(int screenWidth, int screenHeight, bool isVSTNC, HWND hwnd, bool isFullScreen, float depth, float screenNear);
	void ShutDown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* getDeviceContext();

	void GetProjectionMatrix(D3DMATRIX& projection);
	void GetWorldMatrix(D3DMATRIX& world);
	void GetOrthoMatrix(D3DMATRIX& orhto);

	void GetVideoCardInfo(char* cardName, int& memory);


};

