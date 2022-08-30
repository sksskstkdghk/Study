#include "../DxDefine.h"
#include "GraphicsClass.h"

bool GraphicsClass::Render(float rotation)
{
	bool result;

	//���� �� ���� �ʱ�ȭ
	D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	camera->Render();

	camera->GetViewMatrix(viewMatrix);
	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix *= XMMatrixRotationY(rotation);
	//worldMatrix *= XMMatrixScaling(0.1f, 0.1f, 0.1f);

	model->Render(D3D->getDeviceContext());

	result = shaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		model->GetTexture(), light->GetDirection(), light->GetDiffuseColor(), light->GetAmbientColor());
	if (!result)
		return false;

	//���� ����
	D3D->EndScene();

	return true;
}

GraphicsClass::GraphicsClass()
{
	D3D = nullptr;
	camera = nullptr;
	model = nullptr;
	shaderClass = nullptr;
	light = nullptr;
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

	model = new ModelClass();
	if (!model)
	{
		cout << "�� ���� ����\n";
		return false;
	}

	WCHAR* temp = (WCHAR*)L"Resources/Mario.png";
	result = model->Init(D3D->GetDevice(), "Resources/Modeldata/model.txt", temp);
	if (!result)
	{
		cout << "�� �ʱ�ȭ ����\n";
		MessageBox(hwnd, L"Could not init the color shader object.", L"Error", MB_OK);
		return false;
	}

	shaderClass = new LightShaderClass();
	if (!shaderClass)
	{
		cout << "���̴� ���� ����\n";
		return false;
	}

	result = shaderClass->Init(D3D->GetDevice(), hwnd);
	if (!result)
	{
		cout << "���̴� �ʱ�ȭ ����\n";
		MessageBox(hwnd, L"Could not init the color shader object.", L"Error", MB_OK);
		return false;
	}

	light = new LightClass();
	if (!light)
	{
		cout << "���� ���� ����\n";
		return false;
	}

	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}

void GraphicsClass::ShutDown()
{
	if (light)
	{
		delete light;
		light = nullptr;
	}

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

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;

	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	result = Render(rotation);

	if (!result)
	{
		cout << "���� ����\n";
		return false;
	}

	return true;
}
