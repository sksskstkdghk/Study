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

	//���� �븻, ź��Ʈ, ���̳븻 ���
	CalculateModelVectors();

	//���� �ʱ�ȭ
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
	//�ؽ��� ����
	ReleaseTexture();

	//���� ����
	ShutDonwBuffers();

	//�� ����
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

//����, �ε��� ���� �ʱ�ȭ
bool ModelClass::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//����, �ε��� �迭�� ���� ����
	//vertexCount = 3;
	//indexCount = 3;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];
	if (!vertices || !indices)
	{
		cout << "���� �Ǵ� �ε��� �迭 ���� ����\n";
		return false;
	}

	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(model[i].x, model[i].y, model[i].z);
		vertices[i].uv = XMFLOAT2(model[i].u, model[i].v);
		vertices[i].normal = XMFLOAT3(model[i].nx, model[i].ny, model[i].nz);
		vertices[i].tangent = XMFLOAT3(model[i].tx, model[i].ty, model[i].tz);
		vertices[i].binormal = XMFLOAT3(model[i].bx, model[i].by, model[i].bz);

		indices[i] = i;
	}

	/* 
	//���� �Ʒ�
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].uv = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//��� ���
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].uv = XMFLOAT2(0.5f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//������ �Ʒ�
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	//vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].uv = XMFLOAT2(1.0f, 1.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	*/

	//���� ������ description�ۼ�
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//���� �����͸� ����Ű�� ���� ���ҽ� ����ü �ۼ�
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//���� ���� ����
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//�ε��� ������ description�ۼ�
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//�ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü �ۼ�
	indexData.pSysMem = indices;

	//�ε��� ���� ����
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

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);	//���� ���� ����
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);		//�ε��� ���� ����
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	//�ﰢ������ �׸��ڴ�.
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
		cout << "�� ������ ���� ���� ����\n";
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

void ModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex[3];
	XMFLOAT3 tangent, binormal, normal;

	faceCount = vertexCount / 3;
	index = 0;

	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertex[j].x = model[index].x;
			vertex[j].y = model[index].y;
			vertex[j].z = model[index].z;
			vertex[j].u = model[index].u;
			vertex[j].v = model[index].v;
			vertex[j].nx = model[index].nx;
			vertex[j].ny = model[index].ny;
			vertex[j].nz = model[index].nz;

			index++;
		}

		//ź��Ʈ, ���̳븻 ���
		CalculateTnB(vertex, tangent, binormal);
		//�븻 ���
		calculateNormal(tangent, binormal, normal);

		for (int j = 1; j <= 3; j++)
		{
			model[index - j].nx = normal.x;
			model[index - j].ny = normal.y;
			model[index - j].nz = normal.z;
			model[index - j].tx = tangent.x;
			model[index - j].ty = tangent.y;
			model[index - j].tz = tangent.z;
			model[index - j].bx = binormal.x;
			model[index - j].by = binormal.y;
			model[index - j].bz = binormal.z;
		}
	}
}

void ModelClass::CalculateTnB(TempVertexType* vertexs, XMFLOAT3& tangent, XMFLOAT3& binormal)
{
	XMFLOAT3 vector01, vector02;
	XMFLOAT2 u, v;
	float den, length;

	vector01.x = vertexs[1].x - vertexs[0].x;
	vector01.y = vertexs[1].y - vertexs[0].y;
	vector01.z = vertexs[1].z - vertexs[0].z;

	vector02.x = vertexs[2].x - vertexs[0].x;
	vector02.y = vertexs[2].y - vertexs[0].y;
	vector02.z = vertexs[2].z - vertexs[0].z;

	u.x = vertexs[1].u - vertexs[0].u;
	u.y = vertexs[1].v - vertexs[0].v;

	v.x = vertexs[2].u - vertexs[0].u;
	v.y = vertexs[2].v - vertexs[0].v;

	//����: Calculate the denominator of the tangent/binormal equation.
	//������� ����Ʈ(https://scahp.tistory.com/13)
	//���� t,b�� ��� ���� ������������ ����ϴµ�
	//���������İ� ��ġ�ϴ� ��Ľ����� �ٲٰ� �� �� ź��Ʈ �������� ��ȯ�ؾ� ����ε� �븻 ������ �����ϴ�.
	//t,b�� ���� �� ����� ���� ���� e1, e2�� ��Ľ��� ������� ���ؼ� ź��Ʈ ������ ���� �� �ִ�.
	//�ش� ���� ����� ��꿡 �ʿ��ؼ� �����ص� ��
	den = 1.0f / (u.x * v.y - u.y * v.x);

	//ź��Ʈ ���
	tangent.x = (v.y * vector01.x - v.x * vector02.x) * den;
	tangent.y = (v.y * vector01.y - v.x * vector02.y) * den;
	tangent.z = (v.y * vector01.z - v.x * vector02.z) * den;
	//������ ���
	binormal.x = (u.x * vector02.x - u.y * vector01.x) * den;
	binormal.y = (u.x * vector02.y - u.y * vector01.y) * den;
	binormal.z = (u.x * vector02.z - u.y * vector01.z) * den;

	//ź��Ʈ�� ���� ���
	//���� ������ �������̵��� �ȵ��־� ���� ����
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	//ź��Ʈ�� ����ȭ
	//XMFLOAT3�� �� ����ȭ �Լ��� ����... �ٵ� xmvector�� ����ϱ� �ʹ� �����ϰ�... �䵵 ���� ����
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	//�������� ���� ���
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	//�������� ����ȭ
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void ModelClass::calculateNormal(XMFLOAT3 tangent, XMFLOAT3 binormal, XMFLOAT3& normal)
{
	float length;

	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	//����: Normalize the normal and then store it
	//ź��Ʈ�� ����ȭ �ؼ� �����Ѵٴ� �ǹ� �� ��??
	//XMFLOAT3�� �� ����ȭ �Լ��� ����... �ٵ� xmvector�� ����ϱ� �ʹ� �����ϰ�... �䵵 ���� ����
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}
