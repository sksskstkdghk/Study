#include "../DxDefine.h"
#include "GraphicsClass.h"

bool GraphicsClass::Render()
{
	//���� �� ���� �ʱ�ȭ
	D3D->BeginScene(0.5f, 0.5, 0.5f, 1.0f);


	//���� ����
	D3D->EndScene();

	return true;
}

GraphicsClass::GraphicsClass()
{
	D3D = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
	delete D3D;
}

bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	cout << "D3D���� ����\n";
	D3D = new D3DClass();
	if (!D3D)
	{
		cout << "D3D���� ����\n";
		return false;
	}
	cout << "D3D���� ����\n";

	cout << "D3D�ʱ�ȭ ����\n";
	result = D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		cout << "D3D�ʱ�ȭ ����\n";
		return false;
	}
	cout << "D3D�ʱ�ȭ ����\n";

	return true;
}

void GraphicsClass::ShutDown()
{
	if (!D3D)
		return;

	D3D->ShutDown();
	delete D3D;
	D3D = nullptr;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();

	if (!result)
	{
		cout << "���� ����\n";
		return false;
	}

	return true;
}
