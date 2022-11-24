#include "../DxDefine.h"
#include "ModelClass.h"

ModelClass::ModelClass()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;

	//textureClass = nullptr;
	model = nullptr;

	textureArrayClass = nullptr;
}

ModelClass::ModelClass(const ModelClass& modelClass)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Init(ID3D11Device* device, const char* modelDataFileName, WCHAR* fileName)
{
	bool result;

	result = LoadModel(modelDataFileName);
	if (!result)
		return false;

	//버퍼 초기화
	result = InitBuffers(device);
	if (!result)
		return false;

	result = LoadTexture(device, fileName);
	if (!result)
		return false;

	return true;
}

bool ModelClass::AddTexture(ID3D11Device* device, WCHAR* fileName)
{
	bool result;

	result = textureArrayClass->AddTexture(device, fileName);
	if (!result)
		return false;

	return true;
}

void ModelClass::ShutDown()
{
	//텍스쳐 삭제
	ReleaseTexture();

	//버퍼 삭제
	ShutDonwBuffers();

	//모델 삭제
	ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

}

int ModelClass::GetIndexCount()
{
	return indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return textureArrayClass->GetTexture(0);
}

vector<ID3D11ShaderResourceView*> ModelClass::GetTextures()
{
	//return textureClass->GetTexture();
	return textureArrayClass->GetTextures();
}

//정점, 인덱스 버퍼 초기화
bool ModelClass::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//정점, 인덱스 배열의 길이 설정
	//vertexCount = 3;
	//indexCount = 3;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];
	if (!vertices || !indices)
	{
		cout << "정점 또는 인덱스 배열 생성 실패\n";
		return false;
	}

	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(model[i].x, model[i].y, model[i].z);
		vertices[i].uv = XMFLOAT2(model[i].u, model[i].v);
		vertices[i].normal = XMFLOAT3(model[i].nx, model[i].ny, model[i].nz);

		indices[i] = i;
	}

	/* 
	//왼쪽 아래
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].uv = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//상단 가운데
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].uv = XMFLOAT2(0.5f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//오른쪽 아래
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	//vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].uv = XMFLOAT2(1.0f, 1.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	*/

	//정점 버퍼의 description작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//정점 데이터를 가리키는 보조 리소스 구조체 작성
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//정점 버퍼 생성
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//인덱스 버퍼의 description작성
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//인덱스 데이터를 가리키는 보조 리소스 구조체 작성
	indexData.pSysMem = indices;

	//인덱스 버퍼 생성
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void ModelClass::ShutDonwBuffers()
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

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);	//정점 버퍼 세팅
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);		//인덱스 버퍼 세팅
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	//삼각형으로 그리겠다.
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* fileName)
{
	bool result;

	textureArrayClass = new TextureArrayClass();
	if (!textureArrayClass)
		return false;

	result = textureArrayClass->AddTexture(device, fileName);
	if (!result)
		return false;

	/*textureClass = new TextureClass();
	if (!textureClass)
		return false;

	result = textureClass->Init(device, fileName);
	if (!result)
		return false;*/

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (textureArrayClass)
	{
		textureArrayClass->ShutDown();
		delete textureArrayClass;
		textureArrayClass = nullptr;
	}

	/*if (textureClass)
	{
		textureClass->ShutDown();
		delete textureClass;
		textureClass = nullptr;
	}*/
}

bool ModelClass::LoadModel(const char* modelDataFileName)
{
	ifstream fin;
	char input;
	int i;

	fin.open(modelDataFileName);

	if (fin.fail())
	{
		cout << "모델 데이터 파일 오픈 실패\n";
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin >> vertexCount;
	indexCount = vertexCount;

	model = new ModelType[vertexCount];
	if (!model)
		return false;

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i < vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].u >> model[i].v;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	fin.close();


	return true;
}

void ModelClass::ReleaseModel()
{
	if (model)
	{
		delete[] model;
		model = nullptr;
	}
}
