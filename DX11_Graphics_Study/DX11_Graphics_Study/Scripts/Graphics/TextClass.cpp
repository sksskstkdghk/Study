#include "TextClass.h"

TextClass::TextClass()
{
    font = nullptr;
    fontShader = nullptr;

    sentence01 = sentence02 = nullptr;
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::InitSentence(SentenceType** type, int maxLength, ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;

    *type = new SentenceType();
    if (!*type)
        return false;

    (*type)->vertexBuffer = nullptr;
    (*type)->indexBuffer = nullptr;

    (*type)->maxLength = maxLength;

    (*type)->vertexCount = 6 * maxLength;
    (*type)->indexCount = (*type)->vertexCount;

    vertices = new VertexType[(*type)->vertexCount];
    if (!vertices)
        return false;

    indices = new unsigned long[(*type)->indexCount];
    if (!indices)
        return false;

    memset(vertices, 0, sizeof(VertexType) * (*type)->vertexCount);

    for (i = 0; i < (*type)->indexCount; i++)
    {
        indices[i] = i;
    }

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*type)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*type)->vertexBuffer);
    if (FAILED(result))
        return false;

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*type)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*type)->indexBuffer);
    if (FAILED(result))
        return false;

    delete[] vertices;
    delete[] indices;

    vertices = nullptr;
    indices = nullptr;

    return true;
}

bool TextClass::UpdateSentence(SentenceType* type, char* text, int posX, int posY, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
    int numLetters;
    VertexType* vertices, *verticesPtr;
    float drawX, drawY;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT result;

    type->red = red;
    type->green = green;
    type->blue = blue;

    numLetters = (int)strlen(text);

    if (numLetters > type->maxLength)
        return false;

    vertices = new VertexType[type->vertexCount];
    if (!vertices)
        return false;

    memset(vertices, 0, sizeof(VertexType) * type->vertexCount);

    drawX = (float)((screenWidth / 2 * -1) + posX);
    drawY = (float)(screenHeight / 2 - posY);

    font->BuildVertexArray((void*)vertices, text, drawX, drawY);

    //업데이트 버퍼
    result = deviceContext->Map(type->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    verticesPtr = (VertexType*)mappedResource.pData;

    memcpy(verticesPtr, vertices, sizeof(VertexType) * type->vertexCount);

    deviceContext->Unmap(type->vertexBuffer, 0);

    delete[] vertices;
    vertices = nullptr;

    return true;
}

void TextClass::ReleaseSentence(SentenceType** type)
{
    if (*type)
    {
        if ((*type)->vertexBuffer)
        {
            (*type)->vertexBuffer->Release();
            (*type)->vertexBuffer = nullptr;
        }

        if ((*type)->indexBuffer)
        {
            (*type)->indexBuffer->Release();
            (*type)->indexBuffer = nullptr;
        }

        delete (*type);
        (*type) = nullptr;
    }
}

bool TextClass::RenderSentencce(ID3D11DeviceContext* deviceContext, SentenceType* type, XMMATRIX world, XMMATRIX ortho)
{
    unsigned int stride, offset;
    XMFLOAT4 fontColor;
    bool result;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &type->vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(type->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    fontColor = XMFLOAT4(type->red, type->green, type->blue, 1.0f);

    result = fontShader->Render(deviceContext, type->indexCount, world, baseViewMatrix, ortho, font->GetTexture(), fontColor);

    if (!result)
        return false;

    return true;
}

bool TextClass::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX baseViewMatrix)
{
    bool result;

    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    this->baseViewMatrix = baseViewMatrix;

    font = new FontClass();
    if (!font)
        return false;

    result = font->Init(device, (char*)"Resources/FontData/fontdata.txt", (WCHAR*)L"Resources/FontData/font.dds");
    if (!result)
    {
        MessageBox(hwnd, L"Could not init the font object", L"Error", MB_OK);
        return false;
    }

    fontShader = new FontShaderClass();
    if (!fontShader)
        return false;

    result = fontShader->Init(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not init the font shader object", L"Error", MB_OK);
        return false;
    }

    result = InitSentence(&sentence01, 16, device);
    if (!result)
        return false;

    result = UpdateSentence(sentence01, (char*)"Hello", 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
    if (!result)
        return false;

    result = InitSentence(&sentence02, 16, device);
    if (!result)
        return false;

    result = UpdateSentence(sentence02, (char*)"GoodBye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
    if (!result)
        return false;

    return true;
}

void TextClass::ShutDOWN()
{
    ReleaseSentence(&sentence01);
    ReleaseSentence(&sentence02);

    if (fontShader)
    {
        fontShader->ShutDown();
        fontShader = nullptr;
    }

    if (font)
    {
        font->Shutdown();
        font = nullptr;
    }
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX ortho)
{
    bool result;

    result = RenderSentencce(deviceContext, sentence01, world, ortho);
    if (!result)
        return false;

    result = RenderSentencce(deviceContext, sentence02, world, ortho);
    if (!result)
        return false;

    return true;
}
