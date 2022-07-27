#include "../DxDefine.h"
#include "GraphicsClass.h"

bool GraphicsClass::Render()
{
	//랜더 전 버퍼 초기화
	D3D->BeginScene(0.5f, 0.5, 0.5f, 1.0f);


	//버퍼 스왑
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

	cout << "D3D생성 시작\n";
	D3D = new D3DClass();
	if (!D3D)
	{
		cout << "D3D생성 실패\n";
		return false;
	}
	cout << "D3D생성 성공\n";

	cout << "D3D초기화 시작\n";
	result = D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		cout << "D3D초기화 실패\n";
		return false;
	}
	cout << "D3D초기화 성공\n";

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
		cout << "랜더 실패\n";
		return false;
	}

	return true;
}
