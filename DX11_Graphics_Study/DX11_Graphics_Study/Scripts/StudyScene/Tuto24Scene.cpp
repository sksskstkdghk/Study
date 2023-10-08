#include "../DxDefine.h"
#include "Tuto24Scene.h"

Tuto24Scene::Tuto24Scene()
{
    clipplaneShaderClass = nullptr;
}

Tuto24Scene::Tuto24Scene(const Tuto24Scene&)
{
}

Tuto24Scene::~Tuto24Scene()
{
}

bool Tuto24Scene::Init(int screenWidth, int screenHeight, HWND hwnd)
{
    bool result;

    result = __super::Init(screenWidth, screenHeight, hwnd);
    if (!result)
        return false;

    WCHAR* temp = (WCHAR*)L"Resources/Mario.png";
    CreateModel("Resources/square.txt", temp);

    clipplaneShaderClass = new ClipplaneShaderClass();
    if (!clipplaneShaderClass)
        return false;

    result = clipplaneShaderClass->Init(D3D->GetDevice(), hwnd);
    if (!result)
        return false;

    //이 평면은 y축이 0아래인 모든 것을 그리지 않음
    clipPlane = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

    return true;
}

void Tuto24Scene::ShutDown()
{
    if (clipplaneShaderClass)
    {
        clipplaneShaderClass->ShutDown();
        delete clipplaneShaderClass;
        clipplaneShaderClass = nullptr;
    }

    if (model)
    {
        model->ShutDown();
        delete model;
        model = nullptr;
    }

    __super::ShutDown();

}

bool Tuto24Scene::Render()
{
    bool result;

    XMMATRIX S, R;

    D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    camera->Render();

    /*rotation += XM_PI * 0.00005f;
    if (rotation > 360.0f)
        rotation -= 360.0f;*/

    /*S = XMMatrixScaling(5.0f, 5.0f, 5.0f);
    R = XMMatrixRotationY(rotation);

    worldMatrix = S * R;*/

    model->Render(D3D->getDeviceContext());

    result = clipplaneShaderClass->Render(D3D->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTextures(), clipPlane);
    if (!result)
        return false;

    D3D->EndScene();

    return true;
}
