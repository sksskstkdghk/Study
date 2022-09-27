#include "../DxDefine.h"
#include "../Input/InputClass.h"
#include "../Graphics/GraphicsClass.h"
#include "SystemClass.h"

//private-------------------------------------------------------------------
bool SystemClass::Frame()
{
	int mouseX, mouseY;

	//Ű����, ���콺 ���� ����
	if (!input->Frame())
		return false;

	input->GetMouseLocation(mouseX, mouseY);

	if (!graphic->Frame(mouseX, mouseY))
		return false;

	if (!graphic->Render())
		return false;

	return true;
}

void SystemClass::InitWindows(int& width, int& height)
{
	//�ܺ� �����͸� �� ��ü�� ����
	AppHandle = this;

	//�� ���α׷��� �ν��Ͻ��� ������
	hInstance = GetModuleHandle(NULL);

	appName = L"Dx11_Graphics_Study";

	//windowsŬ���� ����
	WNDCLASSEX wc;
	//������ �̵�, ũ�� ���� �� ����, ������ ũ�Ⱑ ����Ǹ� �ٽ� �׸�
	//Ŭ���� ���� �� ��������� ���� DC�� �Ҵ��
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
		cout << "RegisterClass���� ����\n";
		return;

	}

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;

	//Ǯ ��ũ��
	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;	//���� 32��Ʈ
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	//â ���
	else
	{
		width = 1280;
		height = 720;

		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	//������ ���� �� �ڵ� �Ҵ�
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, width, height, NULL, NULL, hInstance, NULL);

	//�����츦 ȭ�鿡 ǥ�� �� ��Ŀ�� ����
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
}

//������ â ����
void SystemClass::ShutDownWindows()
{
	//Ǯ��ũ�� �̶�� ���÷��� ���� �ʱ�ȭ
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//������ â ����
	DestroyWindow(hwnd);
	hwnd = NULL;

	//���α׷� �ν��Ͻ� ����
	UnregisterClass(appName, hInstance);
	hInstance = NULL;

	//�ܺ������� ���� �ʱ�ȭ
	AppHandle = NULL;
}

//public--------------------------------------------------------------------

SystemClass::SystemClass()
{

}

SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Init()
{
	//������ â ����, ���� ����
	int screenWidth = Win_Width;
	int screenHeight = Win_Height;

	//������ ���� �ʱ�ȭ
	InitWindows(screenWidth, screenHeight);

	//input ��ü ����
	input = new InputClass();
	if (!input)
	{
		cout << "��ǲ ���� ����\n";
		return false;
	}

	//input �ʱ�ȭ
	cout << "��ǲ ���� ����\n";
	if (!input->Init(hInstance, hwnd, screenWidth, screenHeight))
	{
		cout << "��ǲ �ʱ�ȭ ����\n";
		return false;
	}

	//graphic ��ü ����
	graphic = new GraphicsClass();
	if (!graphic)
	{
		cout << "�׷��� ���� ����\n";
		return false;
	}

	//graphic ��ü �ʱ�ȭ
	cout << "�׷��� ���� ����\n";
	if (!graphic->Init(screenWidth, screenHeight, hwnd))
	{
		cout << "�׷��� �ʱ�ȭ ����\n";
		return false;
	}

	cout << "�ý��� �ʱ�ȭ ����\n";
	return true;
}

void SystemClass::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		//������ �޼��� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���� �޼����� �޾Ҵٸ� ���� Ż��
		if (msg.message == WM_QUIT)
			break;
		else
		{
			//Frame�Լ� ó��
			if (!Frame())
				break;
		}

		if (input->IsEscapePressed() == true)
			break;
	}

}

void SystemClass::ShutDown()
{
	//graphic ��ȯ
	if (graphic)
	{
		graphic->ShutDown();
		delete graphic;
		graphic = nullptr;
	}

	//input ��ȯ
	if (input)
	{
		input->ShutDown();
		delete input;
		input = nullptr;
	}

	//windows ����
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
		//������ ���� Ȯ��
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		//�����찡 �������� Ȯ��(������ x���� ��������??)
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		//�� ��
		default:
		{
			return AppHandle->MessageHandler(hwnd, umsg, wparam, lparam);
		}
	}
}
