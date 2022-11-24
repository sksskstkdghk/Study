#include "../DxDefine.h"
#include "GraphicsClass.h"

bool GraphicsClass::Render(float rotation)
{
	//랜더 전 버퍼 초기화
	D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera->Render();

	camera->GetViewMatrix(viewMatrix);

	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);
	D3D->GetOrthoMatrix(orthoMatrix);

	model->Render(D3D->getDeviceContext());

	shaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTextures());

	//버퍼 스왑
	D3D->EndScene();

	return true;
}

GraphicsClass::GraphicsClass()
{
	D3D = nullptr;
	camera = nullptr;
	shaderClass = nullptr;
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
	XMMATRIX baseViewMatrix;

	D3D = new D3DClass();
	if (!D3D)
	{
		cout << "D3D생성 실패\n";
		return false;
	}

	result = D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		cout << "D3D초기화 실패\n";
		return false;
	}

	camera = new CameraClass();
	if (!camera)
	{
		cout << "카메라 생성 실패\n";
		return false;
	}

	//카메라 초기 위치 설정
	camera->SetPosition(0.0f, 0.0f, -10.0f);
	camera->Render();
	camera->GetViewMatrix(baseViewMatrix);

	model = new ModelClass();
	if (!model)
	{
		cout << "모델 생성 실패\n";
		return false;
	}

	WCHAR* temp = (WCHAR*)L"Resources/stone01.dds";
	result = model->Init(D3D->GetDevice(), "Resources/square.txt", temp);
	if (!result)
	{
		cout << "모델 초기화 실패\n";
		MessageBox(hwnd, L"Could not init the Model object.", L"Error", MB_OK);
		return false;
	}

	temp = (WCHAR*)L"Resources/dirt01.dds";
	result = model->AddTexture(D3D->GetDevice(), temp);
	if (!result)
	{
		cout << "모델 초기화 실패\n";
		MessageBox(hwnd, L"Could not init the Model object.", L"Error", MB_OK);
		return false;
	}

	shaderClass = new MultiTextureShaderClass();
	if (!shaderClass)
	{
		cout << "셰이더 생성 실패\n";
		return false;
	}

	result = shaderClass->Init(D3D->GetDevice(), hwnd);
	if (!result)
	{
		cout << "셰이더 초기화 실패\n";
		MessageBox(hwnd, L"Could not init the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::ShutDown()
{
	/*if (bitmap)
	{
		bitmap->ShutDown();
		delete bitmap;
		bitmap = nullptr;
	}*/

	if (shaderClass)
	{
		shaderClass->ShutDown();
		delete shaderClass;
		shaderClass = nullptr;
	}

	if (model)
	{
		model->ShutDown();
		delete model;
		model = nullptr;
	}

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
