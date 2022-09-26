#pragma once

class D3DClass
{
private:
	bool isVsync;
	int videoCardMemory;
	char videoCardDescription[128];
	unsigned int numerator = -1, denominator = -1;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState, *depthDisabledStencilState;
	ID3D11BlendState* alphaEnableBlendingState, * alphaDisableBlendingState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;

	XMMATRIX projectionMatrix;
	XMMATRIX worldMatrix;
	XMMATRIX orthoMatrix;

	int screenWidth;	//스크린 너비
	int screenHeight;	//스크린 높이

public:
	D3DClass();
	D3DClass(const D3DClass& d3dClass);
	~D3DClass();

	bool Init(int screenWidth, int screenHeight, bool isVSYNC, HWND hwnd, bool isFullScreen, float screenDepth, float screenNear);
	bool DisplayInit(bool isVSYNC);
	bool BackbufferInit(bool isFullScreen, HWND hwnd);
	bool DepthStencilInit();
	bool DepthDisabledStencilInit();
	bool BlendStateInit();
	bool RasterInit();
	void ViewportInit();
	void ShutDown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* getDeviceContext();

	void GetProjectionMatrix(XMMATRIX& projection);
	void GetWorldMatrix(XMMATRIX& world);
	void GetOrthoMatrix(XMMATRIX& ortho);

	void GetVideoCardInfo(char* cardName, int& memory);

	void TurnZbufferOn();
	void TurnZbufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
};

