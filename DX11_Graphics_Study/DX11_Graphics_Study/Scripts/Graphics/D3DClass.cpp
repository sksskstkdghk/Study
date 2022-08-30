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

#pragma region 초기화
//D3D11 초기화
bool D3DClass::Init(int screenWidth, int screenHeight, bool isVSYNC, HWND hwnd, bool isFullScreen, float screenDepth, float screenNear)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	HRESULT result;
	
	float fieldOfView, screenAspect;

	cout << "화면 초기화 시작\n";
	if (!DisplayInit(isVSYNC))
	{
		cout << "화면 초기화 실패\n";
		return false;
	}
	cout << "화면 초기화 성공\n";

	cout << "백버퍼 초기화 시작\n";
	if (!BackbufferInit(isFullScreen, hwnd))
	{
		cout << "백버퍼 초기화 실패\n";
		return false;
	}
	cout << "백버퍼 초기화 성공\n";

	cout << "깊이-스텐실 초기화 시작\n";
	if (!DepthStencilInit())
	{
		cout << "깊이-스텐실 초기화 실패\n";
		return false;
	}
	cout << "깊이-스텐실 초기화 성공\n";

	cout << "래스터화기 초기화 시작\n";
	if (!RasterInit())
	{
		cout << "래스터화기 초기화 실패\n";
		return false;
	}
	cout << "래스터화기 초기화 성공\n";
	
	cout << "뷰포트 초기화 시작\n";
	ViewportInit();
	cout << "뷰포트 초기화 성공\n";

	//투영 행렬 설정
	fieldOfView = XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	//3D 렌더링을 위한 투영 행렬을 생성
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	//월드 행렬 단위 행렬로 초기화
	worldMatrix = XMMatrixIdentity();
	//2D 렌더링에 사용될 직교 투영 행렬 생성(ui, text)
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

//디스플레이 초기화
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

	//수직동기화 설정
	isVsync = isVSYNC;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		cout << "DX 그래픽 인터페이스 팩토리 생성 실패\n";
		return false;
	}

	//팩토리 객체를 사용하여 첫번째 그래픽 카드 인터페이스에 대한 아담터를 만듬
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		cout << "그래픽 카드 인터페이스 아답터 생성 실패\n";
		return false;
	}

	//모니터에 대한 첫번째 아답터를 나열
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		cout << "모니터에 대한 아답터 나열 실패\n";
		return false;
	}

	//DXGI_FORMAT_R8G8B8A8_UNORM 모니터 출력 디스플레이 포멧에 맞는 모드의 개수를 구함
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		cout << "모니터 출력 디스플레이 모드 개수 가져오기 실패\n";
		return false;
	}

	//가능한 모든 모니터와 그래픽카드 조합을 저장한다.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		cout << "모니터와 그래픽카드 조합 배열 생성 실패\n";
		return false;
	}

	//디스플레이 모드에 대한 리스트 구조를 채움
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		cout << "디스플레이 모드 리스트 설정 실패\n";
		return false;
	}

	//모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾는다.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				//적합한 모드를 찾았다면 모니터의 새로고침 비율 값을 저장
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				break;
			}
	}

	if (numerator == -1 && denominator == -1)
	{
		cout << "화면 너비/높이에 맞는 모드 발견 실패\n";
		return false;
	}

	//어댑터(그래픽카드)의 description을 가져옴
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		cout << "그래픽카드의 desc가져오기 실패\n";
		return false;
	}

	//현재 그래픽카드의 메모리 용량을 메가바이트 단위로 저장
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//그래픽카드의 이름을 char형 문자열 배열로 바꾼 뒤 저장
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		cout << "그래픽카드 이름 변환 및 저장 실패\n";
		return false;
	}

	//디스플레이 설정 후 다시 사용안하니 삭제
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

//백버퍼 초기화 및 랜더타겟 뷰 생성
bool D3DClass::BackbufferInit(bool isFullScreen, HWND hwnd)
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//백 버퍼 설정
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

	//백버퍼의 용도 설정
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//랜더링이 이루어질 윈도우 핸들 설정
	swapChainDesc.OutputWindow = hwnd;
	//멀티샘플링 끔
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//창 모드 또는 전체화면 모드 설정
	if (isFullScreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	//스캔라인의 정렬과 스캔라이닝을 지정되지 않음(unspecified)으로 설정
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//출력된 이후의 백버퍼의 내용을 버리도록 설정
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//추가 옵션 플래그 사용 안함
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel[4] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	//스왑 체인, 디바이스, 디바이스 콘텍스트 생성
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &deviceContext);
	if (FAILED(result))
	{
		cout << "스왑 체인, 디바이스, 디바이스 콘텍스트 생성 실패\n";
		return false;
	}

	//백버퍼의 포인터를 가져옴
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		cout << "백버퍼의 포인터 가져오기 실패\n";
		return false;
	}

	//백버퍼의 포인터로 렌더타겟 뷰를 생성
	result = device->CreateRenderTargetView(backBufferPtr, nullptr, &renderTargetView);
	if (FAILED(result))
	{
		cout << "렌더타겟 뷰 생성 실패\n";
		return false;
	}

	//더이상 사용하지 않는 백버퍼 포인터 해제
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	return true;
}

//깊이-스텐실 초기화
bool D3DClass::DepthStencilInit()
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC depthBuffereDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	//깊이 버퍼의 descroption초기화
	ZeroMemory(&depthBuffereDesc, sizeof(D3D11_TEXTURE2D_DESC));

	//깊이 버퍼의 descroption설정
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

	//깊이 버퍼의 텍스쳐 생성
	result = device->CreateTexture2D(&depthBuffereDesc, nullptr, &depthStencilBuffer);
	if (FAILED(result))
	{
		cout << "깊이 버퍼 텍스쳐 생성 실패\n";
		return false;
	}

	//스텐실 상태 초기화
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//스텐실 상태의 description을 설정
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

	//깊이-스텐실 상태 생성
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result))
	{
		cout << "깊이-스텐실 상태 생성 실패\n";
		return false;
	}

	//깊이-스텐실 상태 설정
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	//깊이-스텐실 뷰의 description초기화
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	//깊이-스텐실 뷰의 description설정
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//깊이-스텐실 뷰 생성
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		cout << "깊이-스텐실 뷰 생성 실패\n";
		return false;
	}

	//렌더타겟 뷰와 깊이-스텐실 버퍼를 각각 출력 파이프라인에 바인딩한다.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return true;
}

//래스터화기 초기화
bool D3DClass::RasterInit()
{
	HRESULT result;

	D3D11_RASTERIZER_DESC rasterDesc;
	
	//어떤 도형을 어떻게 그릴 것인지 결정하는 래스터화기 description을 작성
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

	//작성한 description으로부터 래스터화기 상태를 생성
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		cout << "래스터화기 상태 생성 실패\n";
		return false;
	}

	//래스터화기 상태 설정
	deviceContext->RSSetState(rasterState);

	return true;
}

//뷰포트 초기화
void D3DClass::ViewportInit()
{
	D3D11_VIEWPORT viewPort;

	//렌더링을 위한 뷰포트를 설정
	viewPort.Width = (float)screenWidth;
	viewPort.Height = (float)screenHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	//래스터화기 단계에 뷰포트를 바인딩한다.
	deviceContext->RSSetViewports(1, &viewPort);
}

#pragma endregion

//d3d11 할당 해제
void D3DClass::ShutDown()
{
	//종료 전 윈도우 모드로 바꾸지 않으면 스왑체인을 할당 해제할 때 예외가 발생한다.
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

	//버퍼를 어떤 색상으로 지울 것인지 설정
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//백버퍼 지움
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	//깊이버퍼 지움
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	//렌더링이 완료되었으므로 백버퍼의 내용을 화면에 표시
	if (isVsync)
	{
		//새로고침 비율 고정
		swapChain->Present(1, 0);
	}
	else
	{
		//가능한 빠르게 표시
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
