#include "../DxDefine.h"
#include "GraphicsClass.h"

bool GraphicsClass::Render(float rotation)
{
	return true;
}

bool GraphicsClass::Render()
{
	return true;
}

void GraphicsClass::GetMatrix()
{
}

void GraphicsClass::CreateModel(const char* modelDataFileName, WCHAR* fileName)
{
	model = new ModelClass();
	if (!model)
	{
		cout << "�� ���� ����\n";
		return;
	}

	if (!model->Init(D3D->GetDevice(), modelDataFileName, fileName))
	{
		cout << "�� �ʱ�ȭ ����\n";
		return;
	}
}

bool GraphicsClass::RenderToTexture()
{
	return false;
}

bool GraphicsClass::RenderScene()
{
	return false;
}

GraphicsClass::GraphicsClass()
{
	D3D = nullptr;
	camera = nullptr;
	model = nullptr;
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

	D3D = new D3DClass();
	if (!D3D)
	{
		cout << "D3D���� ����\n";
		return false;
	}

	result = D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		cout << "D3D�ʱ�ȭ ����\n";
		return false;
	}

	camera = new CameraClass();
	if (!camera)
	{
		cout << "ī�޶� ���� ����\n";
		return false;
	}

	//ī�޶� �ʱ� ��ġ ����
	camera->SetPosition(0.0f, 0.0f, -10.0f);
	camera->Render();

	//���� ��� �޾ƿ���
	camera->GetViewMatrix(viewMatrix);
	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);
	D3D->GetOrthoMatrix(orthoMatrix);

	return true;
}

void GraphicsClass::ShutDown()
{
	if (camera)
	{
		delete camera;
		camera = nullptr;
	}

	if (D3D)
	{
		D3D->ShutDown();
		delete D3D;
		D3D = nullptr;
	}
}

bool GraphicsClass::Frame(int fps, int cpuValue, float frameTime)
{
	bool result;

	camera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool GraphicsClass::Frame(float rotationX, float rotationY)
{
	camera->SetPosition(0.0f, 0.0f, -10.0f);

	camera->SetRotation(rotationX, rotationY, 0.0f);

	return true;
}
