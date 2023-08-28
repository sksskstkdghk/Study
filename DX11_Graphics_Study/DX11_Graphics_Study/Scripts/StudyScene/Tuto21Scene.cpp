#include "../DxDefine.h"
#include "Tuto21Scene.h"

Tuto21Scene::Tuto21Scene()
{
	specMapShader = nullptr;
	light = nullptr;
}

Tuto21Scene::Tuto21Scene(const Tuto21Scene& other)
{
}

Tuto21Scene::~Tuto21Scene()
{
}

bool Tuto21Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	result = __super::Init(screenWidth, screenHeight, hwnd);
	if (!result)
		return false;

	model = new ModelClass();
	if (!model)
	{
		cout << "모델 클래스 생성 실패\n";
		return false;
	}

	WCHAR* temp = (WCHAR*)L"Resources/stone02.gif";
	result = model->Init(D3D->GetDevice(), "Resources/ModelData/cube.txt", temp);
	if (!result)
	{
		cout << "모델 초기화 실패\n";
		MessageBox(hwnd, L"Could not init the Model object.", L"Error", MB_OK);
		return false;
	}

	temp = (WCHAR*)L"Resources/stone02_normal.gif";
	result = model->AddTexture(D3D->GetDevice(), temp);
	if (!result)
	{
		cout << "모델 텍스쳐 추가 실패\n";
		return false;
	}

	temp = (WCHAR*)L"Resources/stone02_spec.gif";
	result = model->AddTexture(D3D->GetDevice(), temp);
	if (!result)
	{
		cout << "모델 텍스쳐 추가 실패\n";
		return false;
	}

	specMapShader = new SpecMapShaderClass();
	if (!specMapShader)
	{
		cout << "셰이더 클래스 생성 실패\n";
		return false;
	}

	result = specMapShader->Init(L"Scripts/Shader/HLSL/SpecularMap.hlsl", D3D->GetDevice(), hwnd);
	if (!result)
	{
		cout << "셰이더 초기화 실패\n";
		return false;
	}

	light = new LightClass();
	if (!light)
	{
		cout << "라이트 클래스 생성 실패\n";
		return false;
	}

	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);
	light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetSpecularPower(24.0f);
	light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

void Tuto21Scene::ShutDown()
{
	if (light)
	{
		delete light;
		light = nullptr;
	}

	if (specMapShader)
	{
		delete specMapShader;
		specMapShader = nullptr;
	}

	if (model)
	{
		delete model;
		model = nullptr;
	}

	__super::ShutDown();
}

bool Tuto21Scene::Frame()
{
	return true;
}

bool Tuto21Scene::Render()
{
	D3D->BeginScene(0.0f, 0.0f, 0.0f, 0.0f);

	camera->Render();

	/*rotation += (float)M_PI * 0.00005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;*/

	//worldMatrix = XMMatrixRotationY(rotation);
	model->Render(D3D->getDeviceContext());

	specMapShader->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTextures(), 
		light->GetDirection(), light->GetDiffuseColor(), camera->GetPosition(), light->GetSpecularColor(), light->GetSpecularPower());

	D3D->EndScene();

	return true;
}
