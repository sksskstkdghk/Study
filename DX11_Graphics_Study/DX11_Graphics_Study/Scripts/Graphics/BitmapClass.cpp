#include "../DxDefine.h"
#include "BitmapClass.h"

bool BitmapClass::InitBuffer(ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;

    vertexCount = 6;
    indexCount = vertexCount;

    vertices = new VertexType[vertexCount];
    if (!vertices)
        return false;

    indices = new unsigned long[indexCount];
    if (!indices)
        return false;

    memset(vertices, 0, sizeof(VertexType) * vertexCount);

    for (i = 0; i < indexCount; i++)
    {
        indices[i] = i;
    }

    //버텍스 버퍼 생성
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
    if (FAILED(result))
        return false;

    //인덱스 버퍼 생성
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
    if (FAILED(result))
        return false;

    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

void BitmapClass::ShutDownBuffer()
{
    if (indexBuffer)
    {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }

    if (vertexBuffer)
    {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }
}

bool BitmapClass::UpdateBuffer(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
    if (posX == prevPosX && posY == prevPosY)
        return true;

    //left, right, top, bottom
    float l, r, t, b;
    VertexType* vertices;
    D3D11_MAPPED_SUBRESOURCE mapResources;
    VertexType* verticesPtr;
    HRESULT result;

    prevPosX = posX;
    prevPosY = posY;

    l = (float)((screenWidth  / 2) * -1) + (float)posX - (float)(bitmapWidth / 2);
    r = l + (float)bitmapWidth;
    t = (float)(screenHeight / 2) - (float)posY + (float)(bitmapHeight / 2);
    b = t - (float)bitmapHeight;

    /*l = (float)0 + (float)posX;
    r = l + (float)bitmapWidth;
    t = (float)0 - (float)posY;
    b = t - (float)bitmapHeight;*/

    vertices = new VertexType[vertexCount];
    if (!vertices)
        return false;

    vertices[0].position = XMFLOAT3(l, t, 0.0f);
    vertices[0].uv = XMFLOAT2(0, 0);

    vertices[1].position = XMFLOAT3(r, b, 0.0f);
    vertices[1].uv = XMFLOAT2(1, 1);

    vertices[2].position = XMFLOAT3(l, b, 0.0f);
    vertices[2].uv = XMFLOAT2(0, 1);

    vertices[3].position = XMFLOAT3(l, t, 0.0f);
    vertices[3].uv = XMFLOAT2(0, 0);

    vertices[4].position = XMFLOAT3(r, t, 0.0f);
    vertices[4].uv = XMFLOAT2(1, 0);

    vertices[5].position = XMFLOAT3(r, b, 0.0f);
    vertices[5].uv = XMFLOAT2(1, 1);

    result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResources);
    if (FAILED(result))
        return false;

    verticesPtr = (VertexType*)mapResources.pData;
    memcpy(verticesPtr, (void*)vertices, sizeof(VertexType) * vertexCount);

    deviceContext->Unmap(vertexBuffer, 0);

    delete[] vertices;
    vertices = nullptr;

    return true;
}

void BitmapClass::RenderBuffer(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset = 0;

    stride = sizeof(VertexType);

    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* fileName)
{
    bool result;

    texture = new TextureClass();
    if (!texture)
        return false;

    result = texture->Init(device, fileName);
    if (!result)
        return false;

    return true;
}

void BitmapClass::ReleaseTexture()
{
    if (texture)
    {
        texture->ShutDown();
        delete texture;
        texture = nullptr;
    }
}

BitmapClass::BitmapClass()
{
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    texture = nullptr;
}

BitmapClass::BitmapClass(const BitmapClass& bitmap)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Init(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFileName, int bitmapWidth, int bitmapHeight)
{
    bool result;

    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    this->bitmapWidth = bitmapWidth;
    this->bitmapHeight = bitmapHeight;

    prevPosX = -1;
    prevPosY = -1;

    result = InitBuffer(device);
    if (!result)
        return false;

    result = LoadTexture(device, textureFileName);
    if (!result)
        return false;

    return true;
}

void BitmapClass::ShutDown()
{
    ReleaseTexture();

    ShutDownBuffer();
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
    bool result;

    result = UpdateBuffer(deviceContext, posX, posY);
    if(!result)
        return false;

    RenderBuffer(deviceContext);

    return true;
}

int BitmapClass::GetIndexCount()
{
    return indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
    return texture->GetTexture();
}
