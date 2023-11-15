#include "../DxDefine.h"
#include "Tuto27Scene.h"

Tuto27Scene::Tuto27Scene()
{
	RTT = nullptr;
	reflectionShaderClass = nullptr;
	floorModel = nullptr;
	textureShaderClass = nullptr;
	rotation = 0.0f;
}

Tuto27Scene::Tuto27Scene(const Tuto27Scene&)
{
}

Tuto27Scene::~Tuto27Scene()
{
}

bool Tuto27Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	result = __super::Init(screenWidth, screenHeight, hwnd);
	if (!result)
		return false;

	WCHAR* temp = (WCHAR*)L"Resources/Mario.png";
	CreateModel("Resources/ModelData/cube.txt", temp);

	textureShaderClass = new TextureShaderClass();
	if (!textureShaderClass)
		return false;

	result = textureShaderClass->Init(D3D->GetDevice(), hwnd);
	if (!result)
		return false;

	RTT = new RenderTextureClass();
	if (!RTT)
		return false;

	result = RTT->Init(D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
		return false;

	floorModel = new ModelClass();
	if (!floorModel)
		return false;

	temp = (WCHAR*)L"Resources/dirt01.dds";
	result = floorModel->Init(D3D->GetDevice(), "Resources/ModelData/cube.txt", temp);
	if (!result)
		return false;

	reflectionShaderClass = new ReflectionShaderClass();
	if (!reflectionShaderClass)
		return false;

	result = reflectionShaderClass->Init(D3D->GetDevice(), hwnd);
	if (!result)
		return false;

	return true;
}

void Tuto27Scene::ShutDown()
{
	if (reflectionShaderClass)
	{
		reflectionShaderClass->ShutDown();
		delete reflectionShaderClass;
		reflectionShaderClass = nullptr;
	}

	if (floorModel)
	{
		floorModel->ShutDown();
		delete floorModel;
		floorModel = nullptr;
	}
	
	if (RTT)
	{
		RTT->ShutDown();
		delete RTT;
		RTT = nullptr;
	}

	if (textureShaderClass)
	{
		textureShaderClass->ShutDown();
		delete textureShaderClass;
		textureShaderClass = nullptr;
	}

	__super::ShutDown();
}

bool Tuto27Scene::Render()
{
	bool result;

	result = RenderToTexture();
	if (!result)
		return false;

	result = RenderScene();
	if (!result)
		return false;

	return true;
}

bool Tuto27Scene::RenderToTexture()
{
	bool result;
	XMMATRIX S, R;

	RTT->SetRenderTarget(D3D->getDeviceContext(), D3D->GetDepthStencilView());
	RTT->ClearRenderTarget(D3D->getDeviceContext(), D3D->GetDepthStencilView());

	//장면 그리기 전 높이를 -1.5f로 설정한 반사 행렬 생성
	camera->RenderReflection(-1.5f);

	rotation += XM_PI * 0.00005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	R = XMMatrixRotationY(rotation);
	
	worldMatrix = R;

	model->Render(D3D->getDeviceContext());

	result = textureShaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, camera->GetReflectionViewMatrix(), projectionMatrix, model->GetTexture());
	if (!result)
		return false;

	D3D->SetBackBufferRenderTarget();

	return true;
}

bool Tuto27Scene::RenderScene()
{
	bool result;
	XMMATRIX S, R;

	D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera->Render();

	model->Render(D3D->getDeviceContext());
	floorModel->Render(D3D->getDeviceContext());

	/*result = textureShaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());
	if (!result)
		return false;*/

	S = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	R = XMMatrixRotationY(rotation);

	worldMatrix = S * R * XMMatrixTranslation(0.0f, -1.5f, 0.0f);

	result = reflectionShaderClass->Render(D3D->getDeviceContext(), floorModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, floorModel->GetTextures(), RTT->GetSRV(), camera->GetReflectionViewMatrix());

	D3D->EndScene();

	return true;
}
