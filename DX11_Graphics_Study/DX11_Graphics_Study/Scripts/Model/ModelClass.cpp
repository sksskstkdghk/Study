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

	//모델의 노말, 탄젠트, 바이노말 계산
	CalculateModelVectors();

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
		vertices[i].tangent = XMFLOAT3(model[i].tx, model[i].ty, model[i].tz);
		vertices[i].binormal = XMFLOAT3(model[i].bx, model[i].by, model[i].bz);

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

		//탄젠트, 바이노말 계산
		CalculateTnB(vertex, tangent, binormal);
		//노말 계산
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

	//원문: Calculate the denominator of the tangent/binormal equation.
	//도움받은 사이트(https://scahp.tistory.com/13)
	//백터 t,b를 얻기 위해 연립방정식을 사용하는데
	//연립방정식과 일치하는 행렬식으로 바꾸고 난 뒤 탄젠트 공간으로 변환해야 제대로된 노말 매핑이 가능하다.
	//t,b를 얻을 때 사용한 방향 벡터 e1, e2의 행렬식의 역행렬을 곱해서 탄젠트 공간을 얻을 수 있다.
	//해당 값은 역행렬 계산에 필요해서 저장해둔 값
	den = 1.0f / (u.x * v.y - u.y * v.x);

	//탄젠트 계산
	tangent.x = (v.y * vector01.x - v.x * vector02.x) * den;
	tangent.y = (v.y * vector01.y - v.x * vector02.y) * den;
	tangent.z = (v.y * vector01.z - v.x * vector02.z) * den;
	//종법선 계산
	binormal.x = (u.x * vector02.x - u.y * vector01.x) * den;
	binormal.y = (u.x * vector02.y - u.y * vector01.y) * den;
	binormal.z = (u.x * vector02.z - u.y * vector01.z) * den;

	//탄젠트의 길이 계산
	//따로 연산자 오버라이딩이 안돼있어 각각 곱함
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	//탄젠트를 정규화
	//XMFLOAT3는 왜 정규화 함수가 없냐... 근데 xmvector은 사용하기 너무 불편하고... 답도 없네 ㅋㅋ
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	//종법선의 길이 계산
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	//종법선을 정규화
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

	//원문: Normalize the normal and then store it
	//탄젠트를 정규화 해서 저장한다는 의미 인 듯??
	//XMFLOAT3는 왜 정규화 함수가 없냐... 근데 xmvector은 사용하기 너무 불편하고... 답도 없네 ㅋㅋ
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}
