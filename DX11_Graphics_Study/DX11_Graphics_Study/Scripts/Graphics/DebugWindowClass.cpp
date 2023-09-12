#include "../DxDefine.h"
#include "DebugWindowClass.h"

bool DebugWindowClass::InitBuffers(ID3D11Device* device)
{
	HRESULT result;

	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCnt = 4;
	indexCnt = 6;

	vertices = new VertexType[vertexCnt];
	if (!vertices)
		return false;

	indices = new unsigned long[indexCnt];
	if (!indices)
		return false;

	memset(vertices, 0, (sizeof(VertexType) * vertexCnt));

	vertices[0].position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[0].UV = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[1].UV = XMFLOAT2(1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertices[2].UV = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertices[3].UV = XMFLOAT2(1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 3;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 1;
	indices[5] = 3;

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCnt;
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

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCnt;
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
	delete[] indices;

	vertices = nullptr;
	indices = nullptr;


	return true;
}

void DebugWindowClass::ShutDownBuffers()
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

void DebugWindowClass::UpdateMatrix()
{
	world = S * R * T;
}

void DebugWindowClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

DebugWindowClass::DebugWindowClass()
{
	vertexBuffer = indexBuffer = nullptr;

	T = R = S = XMMatrixIdentity();
	world = XMMatrixIdentity();
}

DebugWindowClass::DebugWindowClass(const DebugWindowClass&)
{
}

DebugWindowClass::~DebugWindowClass()
{
}

bool DebugWindowClass::Init(ID3D11Device* device, int bitMapWidth, int bitMapHeight)
{
	bool result;

	/*this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;*/

	this->bitMapWidth = bitMapWidth;
	this->bitMapHeight = bitMapHeight;

	SetWindowSize(bitMapWidth, bitMapHeight);

	result = InitBuffers(device);
	if (!result)
		return false;

	return true;
}

void DebugWindowClass::ShutDown()
{
	ShutDownBuffers();
}

bool DebugWindowClass::Render(ID3D11DeviceContext* deviceContext)
{
	bool result;

	UpdateMatrix();
	RenderBuffers(deviceContext);

	return true;
}

void DebugWindowClass::SetPostion(float x, float y, float z)
{
	T = XMMatrixTranslation(x, y, z);
}

void DebugWindowClass::SetRotation(float x, float y, float z)
{
	XMVECTOR temp = XMQuaternionRotationRollPitchYaw(x, y, z);

	R = XMMatrixRotationQuaternion(temp);
}

void DebugWindowClass::SetWindowSize(float x, float y, float z)
{
	S = XMMatrixScaling(x, y, z);
}
