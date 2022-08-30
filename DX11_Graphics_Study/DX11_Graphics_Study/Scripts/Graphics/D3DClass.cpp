#include "../DxDefine.h"
#include "D3DClass.h"

D3DClass::D3DClass()
{
	swapChain = nullptr;
	device = nullptr;
	deviceContext = nullptr;
	renderTargetView = nullptr;
	depthStencilBuffer = nullptr;
	depthStencilState = nullptr;
	depthStencilView = nullptr;
	rasterState = nullptr;
}

D3DClass::D3DClass(const D3DClass& d3dClass)
{

}

D3DClass::~D3DClass()
{
}

#pragma region �ʱ�ȭ
//D3D11 �ʱ�ȭ
bool D3DClass::Init(int screenWidth, int screenHeight, bool isVSYNC, HWND hwnd, bool isFullScreen, float screenDepth, float screenNear)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	HRESULT result;
	
	float fieldOfView, screenAspect;

	cout << "ȭ�� �ʱ�ȭ ����\n";
	if (!DisplayInit(isVSYNC))
	{
		cout << "ȭ�� �ʱ�ȭ ����\n";
		return false;
	}
	cout << "ȭ�� �ʱ�ȭ ����\n";

	cout << "����� �ʱ�ȭ ����\n";
	if (!BackbufferInit(isFullScreen, hwnd))
	{
		cout << "����� �ʱ�ȭ ����\n";
		return false;
	}
	cout << "����� �ʱ�ȭ ����\n";

	cout << "����-���ٽ� �ʱ�ȭ ����\n";
	if (!DepthStencilInit())
	{
		cout << "����-���ٽ� �ʱ�ȭ ����\n";
		return false;
	}
	cout << "����-���ٽ� �ʱ�ȭ ����\n";

	cout << "������ȭ�� �ʱ�ȭ ����\n";
	if (!RasterInit())
	{
		cout << "������ȭ�� �ʱ�ȭ ����\n";
		return false;
	}
	cout << "������ȭ�� �ʱ�ȭ ����\n";
	
	cout << "����Ʈ �ʱ�ȭ ����\n";
	ViewportInit();
	cout << "����Ʈ �ʱ�ȭ ����\n";

	//���� ��� ����
	fieldOfView = XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	//3D �������� ���� ���� ����� ����
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	//���� ��� ���� ��ķ� �ʱ�ȭ
	worldMatrix = XMMatrixIdentity();
	//2D �������� ���� ���� ���� ��� ����(ui, text)
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

//���÷��� �ʱ�ȭ
bool D3DClass::DisplayInit(bool isVSYNC)
{
	HRESULT result;
	int error;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;

	//��������ȭ ����
	isVsync = isVSYNC;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		cout << "DX �׷��� �������̽� ���丮 ���� ����\n";
		return false;
	}

	//���丮 ��ü�� ����Ͽ� ù��° �׷��� ī�� �������̽��� ���� �ƴ��͸� ����
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		cout << "�׷��� ī�� �������̽� �ƴ��� ���� ����\n";
		return false;
	}

	//����Ϳ� ���� ù��° �ƴ��͸� ����
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		cout << "����Ϳ� ���� �ƴ��� ���� ����\n";
		return false;
	}

	//DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���信 �´� ����� ������ ����
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		cout << "����� ��� ���÷��� ��� ���� �������� ����\n";
		return false;
	}

	//������ ��� ����Ϳ� �׷���ī�� ������ �����Ѵ�.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		cout << "����Ϳ� �׷���ī�� ���� �迭 ���� ����\n";
		return false;
	}

	//���÷��� ��忡 ���� ����Ʈ ������ ä��
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		cout << "���÷��� ��� ����Ʈ ���� ����\n";
		return false;
	}

	//��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã�´�.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				//������ ��带 ã�Ҵٸ� ������� ���ΰ�ħ ���� ���� ����
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				break;
			}
	}

	if (numerator == -1 && denominator == -1)
	{
		cout << "ȭ�� �ʺ�/���̿� �´� ��� �߰� ����\n";
		return false;
	}

	//�����(�׷���ī��)�� description�� ������
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		cout << "�׷���ī���� desc�������� ����\n";
		return false;
	}

	//���� �׷���ī���� �޸� �뷮�� �ް�����Ʈ ������ ����
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//�׷���ī���� �̸��� char�� ���ڿ� �迭�� �ٲ� �� ����
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		cout << "�׷���ī�� �̸� ��ȯ �� ���� ����\n";
		return false;
	}

	//���÷��� ���� �� �ٽ� �����ϴ� ����
	delete[] displayModeList;
	adapterOutput->Release();
	adapter->Release();
	factory->Release();

	displayModeList = nullptr;
	adapterOutput = nullptr;
	adapter = nullptr;
	factory = nullptr;

	return true;
}

//����� �ʱ�ȭ �� ����Ÿ�� �� ����
bool D3DClass::BackbufferInit(bool isFullScreen, HWND hwnd)
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//�� ���� ����
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (isVsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//������� �뵵 ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//�������� �̷���� ������ �ڵ� ����
	swapChainDesc.OutputWindow = hwnd;
	//��Ƽ���ø� ��
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//â ��� �Ǵ� ��üȭ�� ��� ����
	if (isFullScreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	//��ĵ������ ���İ� ��ĵ���̴��� �������� ����(unspecified)���� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//��µ� ������ ������� ������ �������� ����
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//�߰� �ɼ� �÷��� ��� ����
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel[4] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	//���� ü��, ����̽�, ����̽� ���ؽ�Ʈ ����
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &deviceContext);
	if (FAILED(result))
	{
		cout << "���� ü��, ����̽�, ����̽� ���ؽ�Ʈ ���� ����\n";
		return false;
	}

	//������� �����͸� ������
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		cout << "������� ������ �������� ����\n";
		return false;
	}

	//������� �����ͷ� ����Ÿ�� �並 ����
	result = device->CreateRenderTargetView(backBufferPtr, nullptr, &renderTargetView);
	if (FAILED(result))
	{
		cout << "����Ÿ�� �� ���� ����\n";
		return false;
	}

	//���̻� ������� �ʴ� ����� ������ ����
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	return true;
}

//����-���ٽ� �ʱ�ȭ
bool D3DClass::DepthStencilInit()
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC depthBuffereDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	//���� ������ descroption�ʱ�ȭ
	ZeroMemory(&depthBuffereDesc, sizeof(D3D11_TEXTURE2D_DESC));

	//���� ������ descroption����
	depthBuffereDesc.Width = screenWidth;
	depthBuffereDesc.Height = screenHeight;
	depthBuffereDesc.MipLevels = 1;
	depthBuffereDesc.ArraySize = 1;
	depthBuffereDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBuffereDesc.SampleDesc.Count = 1;
	depthBuffereDesc.SampleDesc.Quality = 0;
	depthBuffereDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBuffereDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBuffereDesc.CPUAccessFlags = 0;
	depthBuffereDesc.MiscFlags = 0;

	//���� ������ �ؽ��� ����
	result = device->CreateTexture2D(&depthBuffereDesc, nullptr, &depthStencilBuffer);
	if (FAILED(result))
	{
		cout << "���� ���� �ؽ��� ���� ����\n";
		return false;
	}

	//���ٽ� ���� �ʱ�ȭ
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//���ٽ� ������ description�� ����
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//����-���ٽ� ���� ����
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result))
	{
		cout << "����-���ٽ� ���� ���� ����\n";
		return false;
	}

	//����-���ٽ� ���� ����
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	//����-���ٽ� ���� description�ʱ�ȭ
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	//����-���ٽ� ���� description����
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//����-���ٽ� �� ����
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		cout << "����-���ٽ� �� ���� ����\n";
		return false;
	}

	//����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε��Ѵ�.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return true;
}

//������ȭ�� �ʱ�ȭ
bool D3DClass::RasterInit()
{
	HRESULT result;

	D3D11_RASTERIZER_DESC rasterDesc;
	
	//� ������ ��� �׸� ������ �����ϴ� ������ȭ�� description�� �ۼ�
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//�ۼ��� description���κ��� ������ȭ�� ���¸� ����
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		cout << "������ȭ�� ���� ���� ����\n";
		return false;
	}

	//������ȭ�� ���� ����
	deviceContext->RSSetState(rasterState);

	return true;
}

//����Ʈ �ʱ�ȭ
void D3DClass::ViewportInit()
{
	D3D11_VIEWPORT viewPort;

	//�������� ���� ����Ʈ�� ����
	viewPort.Width = (float)screenWidth;
	viewPort.Height = (float)screenHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	//������ȭ�� �ܰ迡 ����Ʈ�� ���ε��Ѵ�.
	deviceContext->RSSetViewports(1, &viewPort);
}

#pragma endregion

//d3d11 �Ҵ� ����
void D3DClass::ShutDown()
{
	//���� �� ������ ���� �ٲ��� ������ ����ü���� �Ҵ� ������ �� ���ܰ� �߻��Ѵ�.
	if (swapChain)
		swapChain->SetFullscreenState(false, nullptr);

	if (rasterState)
	{
		rasterState->Release();
		rasterState = nullptr;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	//���۸� � �������� ���� ������ ����
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//����� ����
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	//���̹��� ����
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	//�������� �Ϸ�Ǿ����Ƿ� ������� ������ ȭ�鿡 ǥ��
	if (isVsync)
	{
		//���ΰ�ħ ���� ����
		swapChain->Present(1, 0);
	}
	else
	{
		//������ ������ ǥ��
		swapChain->Present(0, 0);
	}
}

ID3D11Device* D3DClass::GetDevice()
{
	return device;
}

ID3D11DeviceContext* D3DClass::getDeviceContext()
{
	return deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX& projection)
{
	projection = projectionMatrix;
}

void D3DClass::GetWorldMatrix(XMMATRIX& world)
{
	world = worldMatrix;
}

void D3DClass::GetOrthoMatrix(XMMATRIX& ortho)
{
	ortho = orthoMatrix;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
}
