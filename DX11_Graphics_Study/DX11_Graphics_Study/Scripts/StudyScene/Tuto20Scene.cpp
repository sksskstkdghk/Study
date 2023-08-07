#include "../DxDefine.h"
#include "Tuto20Scene.h"

Tuto20Scene::Tuto20Scene()
{
	bumpMapShader = nullptr;
	light = nullptr;
}

Tuto20Scene::Tuto20Scene(const Tuto20Scene& other)
{
}

Tuto20Scene::~Tuto20Scene()
{
}

bool Tuto20Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	result = __super::Init(screenWidth, screenHeight, hwnd);
	if (!result)
		return false;

	model = new ModelClass();
	if (!model)
	{
		cout << "�� Ŭ���� ���� ����\n";
		return false;
	}

	WCHAR* temp = (WCHAR*)L"Resources/stone01.dds";
	result = model->Init(D3D->GetDevice(), "Resources/ModelData/cube.txt", temp);
	if (!result)
	{
		cout << "�� �ʱ�ȭ ����\n";
		MessageBox(hwnd, L"Could not init the Model object.", L"Error", MB_OK);
		return false;
	}

	temp = (WCHAR*)L"Resources/stone01_normal.gif";
	result = model->AddTexture(D3D->GetDevice(), temp);
	if (!result)
	{
		cout << "�� �ؽ��� �߰� ����\n";
		return false;
	}

	bumpMapShader = new BumpMapShaderClass();
	if (!bumpMapShader)
	{
		cout << "���̴� Ŭ���� ���� ����\n";
		return false;
	}

	result = bumpMapShader->Init(D3D->GetDevice(), hwnd);
	if (!result)
	{
		cout << "���̴� �ʱ�ȭ ����\n";
		return false;
	}

	light = new LightClass();
	if (!light)
	{
		cout << "����Ʈ Ŭ���� ���� ����\n";
		return false;
	}

	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void Tuto20Scene::ShutDown()
{
	if (light)
	{
		delete light;
		light = nullptr;
	}

	if (bumpMapShader)
	{
		bumpMapShader->ShutDown();
		delete bumpMapShader;
		light = nullptr;
	}

	if (model)
	{
		model->ShutDown();
		delete model;
		light = nullptr;
	}

	__super::ShutDown();
}

bool Tuto20Scene::Frame()
{
	return true;
}

bool Tuto20Scene::Render()
{
	D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	camera->Render();

	rotation += (float)M_PI * 0.00005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	worldMatrix = XMMatrixRotationY(rotation);

	model->Render(D3D->getDeviceContext());

	bumpMapShader->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTextures(), light->GetDirection(), light->GetDiffuseColor());
	D3D->EndScene();

	return true;
}
