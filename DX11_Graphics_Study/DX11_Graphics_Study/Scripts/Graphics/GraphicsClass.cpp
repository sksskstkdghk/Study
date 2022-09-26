#include "../DxDefine.h"
#include "GraphicsClass.h"

bool GraphicsClass::Render(float rotation)
{
	bool result;

	//���� �� ���� �ʱ�ȭ
	D3D->BeginScene(0.0f, 1.0f, 0.0f, 1.0f);

	camera->Render();

	camera->GetViewMatrix(viewMatrix);
	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);

	D3D->GetOrthoMatrix(orthoMatrix);

	D3D->TurnZbufferOff();

	D3D->TurnOnAlphaBlending();

	result = text->Render(D3D->getDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
		return false;
	D3D->TurnOffAlphaBlending();
	D3D->TurnZbufferOn();
	/*
	worldMatrix = XMMatrixIdentity();
	result = bitmap->Render(D3D->getDeviceContext(), 500, 100);
	textureClass->Render(D3D->getDeviceContext(), bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, bitmap->GetTexture());
	
	D3D->TurnZbufferOn();

	worldMatrix *= XMMatrixRotationY(rotation);
	worldMatrix *= XMMatrixScaling(0.1f, 0.1f, 0.1f);

	model->Render(D3D->getDeviceContext());

	result = shaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		model->GetTexture(), light->GetDirection(), light->GetDiffuseColor(), light->GetAmbientColor(), 
		camera->GetPosition(), light->GetSpecularColor(), light->GetSpecularPower());*/

	//if (!result)
	//	return false;

	//���� ����
	D3D->EndScene();

	return true;
}

GraphicsClass::GraphicsClass()
{
	D3D = nullptr;
	camera = nullptr;
	//model = nullptr;
	//shaderClass = nullptr;
	//textureClass = nullptr;
	//light = nullptr;
	//bitmap = nullptr;
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
	camera->GetViewMatrix(baseViewMatrix);

	text = new TextClass();
	if (!text)
	{
		cout << "�ؽ�Ʈ Ŭ���� ���� ����\n";
		return false;
	}

	result = text->Init(D3D->GetDevice(), D3D->getDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		cout << "�ؽ�Ʈ Ŭ���� �ʱ�ȭ ����\n";
		MessageBox(hwnd, L"Could not init the Text object.", L"Error", MB_OK);
		return false;
	}

	/*model = new ModelClass();
	if (!model)
	{
		cout << "�� ���� ����\n";
		return false;
	}

	WCHAR* temp = (WCHAR*)L"Resources/Mario.png";
	result = model->Init(D3D->GetDevice(), "Resources/Modeldata/Sphere.txt", temp);
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
	light->SetSpecularColor(0.0f, 0.0f, 1.0f, 1.0f);
	light->SetSpecularPower(8.0f);
	

	textureClass = new TextureShaderClass();
	textureClass->Init(D3D->GetDevice(), hwnd);

	bitmap = new BitmapClass();
	if (!bitmap)
	{
		cout << "��Ʈ�� ���� ����\n";
		return false;
	}

	WCHAR* temp = (WCHAR*)L"Resources/FontData/font.dds";
	result = bitmap->Init(D3D->GetDevice(), screenWidth, screenHeight, temp, 1200, 50);
	if (!result)
	{
		cout << "��Ʈ�� �ʱ�ȭ ����\n";
		return false;
	}
	*/

	return true;
}

void GraphicsClass::ShutDown()
{
	/*if (bitmap)
	{
		bitmap->ShutDown();
		delete bitmap;
		bitmap = nullptr;
	}

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
	}*/

	if (text)
	{
		text->ShutDOWN();
		delete text;
		text = nullptr;
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
