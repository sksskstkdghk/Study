#include "../DxDefine.h"
#include "Tuto16Scene.h"

bool Tuto16Scene::Render(float rotation)
{
	bool renderModel, result;

	int modelCount, renderCount;
	float radius;
	XMFLOAT3 pos;
	XMCOLOR color;

	//���� �� ���� �ʱ�ȭ
	D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera->Render();

	camera->GetViewMatrix(viewMatrix);

	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);
	D3D->GetOrthoMatrix(orthoMatrix);

	frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	modelCount = modelList->GetModelCount();
	renderCount = 0;

	for (int i = 0; i < modelCount; i++)
	{
		modelList->GetData(i, pos, color);

		radius = 1.0f;

		renderModel = frustum->CheckSphere(pos.x, pos.y, pos.z, radius);

		if (renderModel)
		{
			worldMatrix *= XMMatrixTranslation(pos.x, pos.y, pos.z);

			model->Render(D3D->getDeviceContext());

			shaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				model->GetTexture(), light->GetDirection(), light->GetDiffuseColor(), light->GetAmbientColor(),
				camera->GetPosition(), light->GetSpecularColor(), light->GetSpecularPower());

			D3D->GetWorldMatrix(worldMatrix);

			renderCount++;
		}
	}

	/*shaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		model->GetTexture(), light->GetDirection(), light->GetDiffuseColor(), light->GetAmbientColor(),
		camera->GetPosition(), light->GetSpecularColor(), light->GetSpecularPower());*/

	result = text->SetRenderCount(renderCount, D3D->getDeviceContext());
	if (!result)
		return false;

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

Tuto16Scene::Tuto16Scene()
{
	D3D = nullptr;
	camera = nullptr;
	model = nullptr;
	modelList = nullptr;
	shaderClass = nullptr;
	text = nullptr;
	//textureClass = nullptr;
	light = nullptr;
	//bitmap = nullptr;
	frustum = nullptr;
}

Tuto16Scene::Tuto16Scene(const Tuto16Scene& other)
{
}

Tuto16Scene::~Tuto16Scene()
{
	delete D3D;
}

bool Tuto16Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
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
	light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetSpecularPower(8.0f);

	modelList = new ModelListClass();
	if (!modelList)
	{
		cout << "�𵨸���Ʈ ���� ����\n";
		return false;
	}

	result = modelList->Init(25);
	if (!result)
	{
		cout << "�𵨸���Ʈ �ʱ�ȭ ����\n";
		MessageBox(hwnd, L"Could not init the modelList object.", L"Error", MB_OK);
		return false;
	}

	frustum = new FrustumClass();
	if (!frustum)
	{
		cout << "�������� ���� ����\n";
		return false;
	}

	/*textureClass = new TextureShaderClass();
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
	}*/


	return true;
}

void Tuto16Scene::ShutDown()
{
	/*if (bitmap)
	{
		bitmap->ShutDown();
		delete bitmap;
		bitmap = nullptr;
	}*/

	if (frustum)
	{
		delete frustum;
		frustum = nullptr;
	}

	if (modelList)
	{
		modelList->ShutDown();
		delete modelList;
		modelList = nullptr;
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
	}

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

bool Tuto16Scene::Frame(int fps, int cpuValue, float frameTime)
{
	bool result;

	/*static float rotation = 0.0f;
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;*/
		//result = Render(rotation);

		//result = text->SetMousePosition(mouseX, mouseY, D3D->getDeviceContext());

	result = text->SetFps(fps, D3D->getDeviceContext());
	if (!result)
	{
		cout << "���� ����\n";
		return false;
	}

	result = text->SetCpu(cpuValue, D3D->getDeviceContext());
	if (!result)
	{
		cout << "���� ����\n";
		return false;
	}

	camera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool Tuto16Scene::Frame(float rotationX, float rotationY)
{
	camera->SetPosition(0.0f, 0.0f, -10.0f);

	camera->SetRotation(rotationX, rotationY, 0.0f);

	return true;
}
