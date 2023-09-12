#include "../DxDefine.h"
#include "Tuto22Scene.h"

bool Tuto22Scene::RenderToTexture()
{
	bool result;

	renderTexture->SetRenderTarget(D3D->getDeviceContext(), D3D->GetDepthStencilView());
	renderTexture->ClearRenderTarget(D3D->getDeviceContext(), D3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	//여기서 씬을 그리면 백버퍼 대신 RTT에 랜더링됨
	result = RenderScene();
	if (!result)
		return false;

	//RTT에서 다시 백버퍼로 되돌림
	D3D->SetBackBufferRenderTarget();

	return true;
}

bool Tuto22Scene::RenderScene()
{
	bool result;

	camera->Render();

	/*D3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);*/

	rotation += XM_PI * 0.00005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	worldMatrix = XMMatrixRotationY(rotation);

	model->Render(D3D->getDeviceContext());

	result = lightShader->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture(),
								light->GetDirection(), light->GetDiffuseColor(), light->GetAmbientColor(), camera->GetPosition(), light->GetSpecularColor(), light->GetSpecularPower());
	if (!result)
		return false;

	return true;
}

Tuto22Scene::Tuto22Scene()
{
	lightShader = nullptr;
	light = nullptr;
	renderTexture = nullptr;
	debugWindow = nullptr;
	textureShader = nullptr;
}

Tuto22Scene::Tuto22Scene(const Tuto22Scene& other)
{

}

Tuto22Scene::~Tuto22Scene()
{

}

bool Tuto22Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	__super::Init(screenWidth, screenHeight, hwnd);

	WCHAR* temp = (WCHAR*)L"Resources/Mario.png";
	CreateModel("Resources/ModelData/cube.txt", temp);

	lightShader = new LightShaderClass();
	if (!lightShader)
		return false;

	result = lightShader->Init(D3D->GetDevice(), hwnd);
	if (!result)
		return false;

	light = new LightClass();
	if (!light)
		return false;

	light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);

	renderTexture = new RenderTextureClass();
	if (!renderTexture)
		return false;

	result = renderTexture->Init(D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
		return false;

	debugWindow = new DebugWindowClass();
	if (!debugWindow)
		return false;

	result = debugWindow->Init(D3D->GetDevice(), screenWidth, screenHeight, 300, 600);
	if (!result)
		return false;

	textureShader = new TextureShaderClass();
	if (!textureShader)
		return false;

	result = textureShader->Init(D3D->GetDevice(), hwnd);
	if (!result)
		return false;

	return true;
}

void Tuto22Scene::ShutDown()
{
	if (textureShader)
	{
		textureShader->ShutDown();
		delete textureShader;
		textureShader = nullptr;
	}

	// 디버그 윈도우를 해제합니다.
	if (debugWindow)
	{
		debugWindow->ShutDown();
		delete debugWindow;
		debugWindow = nullptr;
	}

	// RTT 객체를 해제합니다.
	if (renderTexture)
	{
		renderTexture->ShutDown();
		delete renderTexture;
		renderTexture = nullptr;
	}

	// Release the light object.
	if (light)
	{
		delete light;
		light = nullptr;
	}

	// Release the light shader object.
	if (lightShader)
	{
		lightShader->ShutDown();
		delete lightShader;
		lightShader = nullptr;
	}

	// Release the model object.
	if (model)
	{
		model->ShutDown();
		delete model;
		model = nullptr;
	}

	__super::ShutDown();
}

bool Tuto22Scene::Frame()
{
	return true;
}

bool Tuto22Scene::Render()
{
	bool result;

	result = RenderToTexture();
	if (!result)
		return false;

	D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	result = RenderScene();
	if (!result)
		return false;

	D3D->TurnZbufferOff();

	D3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	D3D->GetOrthoMatrix(orthoMatrix);

	debugWindow->SetPostion(-300, 0);
	result = debugWindow->Render(D3D->getDeviceContext());
	if (!result)
		return false;

	result = textureShader->Render(D3D->getDeviceContext(), debugWindow->GetIndexCount(), debugWindow->GetWorld(), viewMatrix, orthoMatrix, renderTexture->GetSRV());
	if (!result)
		return false;

	D3D->TurnZbufferOn();

	D3D->EndScene();

	return true;
}
