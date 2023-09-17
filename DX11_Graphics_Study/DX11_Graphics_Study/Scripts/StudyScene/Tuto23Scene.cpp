#include "../DxDefine.h"
#include "Tuto23Scene.h"

Tuto23Scene::Tuto23Scene()
{
    fogShaderClass = nullptr;
    rotation = 0.0f;
}

Tuto23Scene::Tuto23Scene(const Tuto23Scene&)
{
}

Tuto23Scene::~Tuto23Scene()
{
}

bool Tuto23Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
{
    bool result;

    result = __super::Init(screenWidth, screenHeight, hwnd);
    if (!result)
        return false;

    WCHAR* temp = (WCHAR*)L"Resources/Mario.png";
    CreateModel("Resources/ModelData/cube.txt", temp);

    fogShaderClass = new FogShaderClass();
    if (!fogShaderClass)
        return false;

    result = fogShaderClass->Init(D3D->GetDevice(), hwnd);
    if (!result)
        return false;

    return true;
}

void Tuto23Scene::ShutDown()
{
    if (fogShaderClass)
    {
        fogShaderClass->ShutDown();
        delete fogShaderClass;
        fogShaderClass = nullptr;
    }

    if (model)
    {
        model->ShutDown();
        delete model;
        model = nullptr;
    }

    __super::ShutDown();
}

bool Tuto23Scene::Render()
{
    float fogColor, fogStart, fogEnd;
    bool result;

    fogColor = 0.5f;
    fogStart = 0.0f;
    fogEnd = 10.0f;

    //백버퍼를 안개 색상으로 초기화하는데 안개 구현에 있어서 매우 중요한 부분이라 함
    //근데 그 이유는 안알려줌;; 아니 뭐 필요한 만큼은 보여줬다 이거임??
    //초기화 색상 바꿔서 확인해보니 백버퍼 초기화를 안개 색과 동일하게 해야 안개 효과를 받은 것 처럼 오브젝트가 보여짐
    //제대로 초기화를 안한다면 굉장히 이질적으로 보인다.
    D3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);
    //D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    camera->Render();

    rotation += XM_PI * 0.00005f;
    if (rotation > 360.0f)
        rotation -= 360.0f;

    worldMatrix = XMMatrixRotationY(rotation);

    model->Render(D3D->getDeviceContext());

    result = fogShaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture(), fogStart, fogEnd);
    if (!result)
        return false;

    D3D->EndScene();

    return true;
}
