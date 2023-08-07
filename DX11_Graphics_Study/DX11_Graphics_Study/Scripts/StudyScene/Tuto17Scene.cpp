#include "../DxDefine.h"
#include "Tuto17Scene.h"

bool Tuto17Scene::Render(float rotation)
{
	//랜더 전 버퍼 초기화
	D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera->Render();

	model->Render(D3D->getDeviceContext());

	shaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTextures());

	//버퍼 스왑
	D3D->EndScene();

	return true;
}

Tuto17Scene::Tuto17Scene()
{
	D3D = nullptr;
	camera = nullptr;
	shaderClass = nullptr;
	//model = nullptr;
}

Tuto17Scene::Tuto17Scene(const Tuto17Scene& other)
{
}

Tuto17Scene::~Tuto17Scene()
{
	delete D3D;
}

bool Tuto17Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	
	result = __super::Init(screenWidth, screenHeight, hwnd);
	if (!result)
		return false;

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

	result = shaderClass->Init(L"Scripts/Shader/HLSL/MultiTexture.hlsl", D3D->GetDevice(), hwnd);
	if (!result)
	{
		cout << "셰이더 초기화 실패\n";
		MessageBox(hwnd, L"Could not init the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Tuto17Scene::ShutDown()
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

	__super::ShutDown();
}