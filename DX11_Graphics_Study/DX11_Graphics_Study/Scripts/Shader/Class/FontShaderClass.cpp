#include "../../DxDefine.h"
#include "FontShaderClass.h"

bool FontShaderClass::InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename)
{
    HRESULT result;
    D3D11_BUFFER_DESC pixelBufferDesc;

    __super::InitShader(device, hwnd, shaderFilename);

    pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
    pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    pixelBufferDesc.MiscFlags = 0;
    pixelBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&pixelBufferDesc, nullptr, &pixelBuffer);
    if (FAILED(result))
        return false;

    return true;
}

void FontShaderClass::ShutDownShader()
{
    if (pixelBuffer)
    {
        pixelBuffer->Release();
        pixelBuffer = nullptr;
    }

    __super::ShutDownShader();
}

bool FontShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 fontColor)
{
    HRESULT result;
    PixelBufferType* dataPtr2;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    __super::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);

    result = deviceContext->Map(pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;

    dataPtr2 = (PixelBufferType*)mappedResource.pData;

    dataPtr2->fontColor = fontColor;

    deviceContext->Unmap(pixelBuffer, 0);

    deviceContext->PSSetConstantBuffers(1, 1, &pixelBuffer);

    return true;
}

FontShaderClass::FontShaderClass()
{
	pixelBuffer = nullptr;
}

FontShaderClass::FontShaderClass(const FontShaderClass& other)
{

}

FontShaderClass::~FontShaderClass()
{

}

bool FontShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
    bool result;

    result = InitShader(device, hwnd, L"Scripts/Shader/HLSL/Font.hlsl");
    if (!result)
        return false;

    return true;
}

void FontShaderClass::ShutDown()
{
    ShutDownShader();
}

bool FontShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 fontColor)
{
    bool result;

    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, fontColor);
    if (!result)
        return false;

    RenderShader(deviceContext, indexCount);

    return true;
}
