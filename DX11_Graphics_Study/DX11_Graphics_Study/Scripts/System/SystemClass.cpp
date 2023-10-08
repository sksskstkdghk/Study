#include "../DxDefine.h"
#include "../Input/InputClass.h"
#include "../Graphics/GraphicsClass.h"
#include "../Sound/SoundClass.h"
#include "SystemClass.h"

//private-------------------------------------------------------------------
bool SystemClass::Frame()
{
	int mouseX, mouseY;
	float x, y;

	timer->Frame();
	fps->Frame();
	cpuClass->Frame();

	//키보드, 마우스 상태 갱신
	if (!input->Frame())
		return false;

	input->GetMouseLocation(mouseX, mouseY);
	position->SetFrameTime(timer->GetTime());

	position->TurnLeft(input->isKeyDown(DIK_LEFTARROW));
	position->TurnRight(input->isKeyDown(DIK_RIGHTARROW));
	position->TurnUp(input->isKeyDown(DIK_UPARROW));
	position->TurnDown(input->isKeyDown(DIK_DOWNARROW));

	position->GetRotation(x, y);

	if (!graphic->Frame(x, y))
		return false;

	/*if (!graphic->Frame(fps->GetFps(), cpuClass->GetCpuPercentage(), timer->GetTime()))
		return false;*/

	if (!graphic->Render())
		return false;

	return true;
}

void SystemClass::InitWindows(int& width, int& height)
{
	//외부 포인터를 이 객체로 지정
	AppHandle = this;

	//이 프로그램의 인스턴스를 가져옴
	hInstance = GetModuleHandle(NULL);

	appName = L"Dx11_Graphics_Study";

	//windows클래스 설정
	WNDCLASSEX wc;
	//윈도우 이동, 크기 조절 시 가로, 세로의 크기가 변경되면 다시 그림
	//클래스 내부 각 윈도우들이 고유 DC가 할당됨
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	ATOM atom = RegisterClassEx(&wc);

	if (atom == 0)
	{
		cout << "RegisterClass생성 실패\n";
		return;

	}

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;

	//풀 스크린
	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;	//색상 32비트
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	//창 모드
	else
	{
		width = 1280;
		height = 720;

		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	//윈도우 생성 및 핸들 할당
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, width, height, NULL, NULL, hInstance, NULL);

	//윈도우를 화면에 표시 및 포커스 지정
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
}

//윈도우 창 종료
void SystemClass::ShutDownWindows()
{
	//풀스크린 이라면 디스플레이 설정 초기화
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//윈도우 창 제거
	DestroyWindow(hwnd);
	hwnd = NULL;

	//프로그램 인스턴스 제거
	UnregisterClass(appName, hInstance);
	hInstance = NULL;

	//외부포인터 참조 초기화
	AppHandle = NULL;
}

//public--------------------------------------------------------------------

SystemClass::SystemClass()
{
	fps = nullptr;
	cpuClass = nullptr;
	timer = nullptr;
	position = nullptr;
}

SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Init()
{
	//윈도우 창 가로, 세로 넓이
	int screenWidth = Win_Width;
	int screenHeight = Win_Height;

	//윈도우 생성 초기화
	InitWindows(screenWidth, screenHeight);

	//input 객체 생성
	input = new InputClass();
	if (!input)
	{
		cout << "인풋 생성 실패\n";
		return false;
	}

	//input 초기화
	cout << "인풋 생성 성공\n";
	if (!input->Init(hInstance, hwnd, screenWidth, screenHeight))
	{
		cout << "인풋 초기화 실패\n";
		return false;
	}

	//graphic 객체 생성
	graphic = new Tuto24Scene();
	if (!graphic)
	{
		cout << "그래픽 생성 실패\n";
		return false;
	}

	//graphic 객체 초기화
	cout << "그래픽 생성 성공\n";
	if (!graphic->Init(screenWidth, screenHeight, hwnd))
	{
		cout << "그래픽 초기화 실패\n";
		return false;
	}

	//sound 객체 생성
	sound = new SoundClass();
	if (!sound)
	{
		cout << "사운드 생성 실패\n";
		return false;
	}

	//sound 객체 초기화
	cout << "사운드 생성 성공\n";
	if (!sound->Init(hwnd))
	{
		cout << "사운드 초기화 실패\n";
		return false;
	}

	fps = new FpsClass();
	if (!fps)
	{
		cout << "fps 생성 실패\n";
		return false;
	}

	//fps 객체 초기화
	cout << "fps 생성 성공\n";
	fps->Init();

	cpuClass = new CpuClass();
	if (!cpuClass)
	{
		cout << "cpu 생성 실패\n";
		return false;
	}

	//cpu 객체 초기화
	cout << "cpu 생성 성공\n";
	cpuClass->Init();

	timer = new TimerClass();
	if (!timer)
	{
		cout << "timer 생성 실패\n";
		return false;
	}

	//timer 객체 초기화
	cout << "timer 생성 성공\n";
	if (!timer->Init())
	{
		cout << "timer 초기화 실패\n";
		return false;
	}

	position = new PositionClass();
	if (!position)
	{
		cout << "position 생성 실패\n";
		return false;
	}

	cout << "시스템 초기화 성공\n";
	return true;
}

void SystemClass::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		//윈도우 메세지 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//종료 메세지를 받았다면 루프 탈출
		if (msg.message == WM_QUIT)
			break;
		else
		{
			//Frame함수 처리
			if (!Frame())
				break;
		}

		if (input->IsEscapePressed() == true)
			break;
	}

}

void SystemClass::ShutDown()
{
	if (position)
	{
		delete position;
		position = nullptr;
	}

	if (timer)
	{
		delete timer;
		timer = nullptr;
	}

	if (cpuClass)
	{
		cpuClass->Shutdown();
		delete cpuClass;
		cpuClass = nullptr;
	}

	if (fps)
	{
		delete fps;
		fps = nullptr;
	}

	//graphic 반환
	if (graphic)
	{
		graphic->ShutDown();
		delete graphic;
		graphic = nullptr;
	}

	//input 반환
	if (input)
	{
		input->ShutDown();
		delete input;
		input = nullptr;
	}

	if (sound)
	{
		sound->ShutDown();
		delete sound;
		sound = nullptr;
	}

	//windows 종료
	ShutDownWindows();
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//윈도우 종료 확인
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		//윈도우가 닫히는지 확인(일종의 x누른 강제종료??)
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		//그 외
		default:
		{
			return AppHandle->MessageHandler(hwnd, umsg, wparam, lparam);
		}
	}
}
