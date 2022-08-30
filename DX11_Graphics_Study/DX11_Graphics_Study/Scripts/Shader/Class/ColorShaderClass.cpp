#include "../../DxDefine.h"
#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	matrixBuffer = nullptr;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& colorShader)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, L"Scripts/Shader/HLSL/Color.hlsl");
	if (!result)
		return false;

	return true;
}

void ColorShaderClass::ShutDown()
{
	ShutDownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShaderClass::InitShader(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;

	vertexShaderBlob = nullptr;
	pixelShaderBlob = nullptr;

	if (!CreateVSnPS(device, hwnd, shaderFilename, &vertexShaderBlob, &pixelShaderBlob))
		return false;

	if (!CreateLayout(device, &vertexShaderBlob, &pixelShaderBlob))
		return false;

	//����� ���� ���� ����
	vertexShaderBlob->Release();
	pixelShaderBlob->Release();

	vertexShaderBlob = nullptr;
	pixelShaderBlob = nullptr;

	if (!CreateWVPConstBuffer(device))
		return false;

	return true;
}

//���̴� ������ �� ����
bool ColorShaderClass::CreateVSnPS(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	errorMessage = nullptr;

	//���� ���̴��� ������
	result = D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &(*BSBlob), &errorMessage);
	if (FAILED(result))
	{
		cout << "���� ���̴� ������ ����\n";

		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, shaderFilename);
		}
		else
		{
			MessageBox(hwnd, shaderFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//�ȼ� ���̴� ������
	result = D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &(*PSBlob), &errorMessage);
	if (FAILED(result))
	{
		cout << "�ȼ� ���̴� ������ ����\n";

		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, shaderFilename);
		}
		else
		{
			MessageBox(hwnd, shaderFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//���۷κ��� ���� ���̴��� ����
	result = device->CreateVertexShader((*BSBlob)->GetBufferPointer(), (*BSBlob)->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(result))
		return false;

	//���۷κ��� �ȼ� ���̴��� ����
	result = device->CreatePixelShader((*PSBlob)->GetBufferPointer(), (*PSBlob)->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(result))
		return false;

	return true;
}

//���̾ƿ� ����
bool ColorShaderClass::CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	HRESULT result;

	unsigned const int numElements = 2;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[numElements];

	//��ġ ������ ����
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	//�÷� ������ ����
	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//���̾ƿ� ����
	result = device->CreateInputLayout(polygonLayout, numElements, (*BSBlob)->GetBufferPointer(), (*BSBlob)->GetBufferSize(), &layout);
	if (FAILED(result))
	{
		cout << "���̾ƿ� ���� ����\n";
		return false;
	}

	return true;
}

//����, ��, �������� ��� ���� ����
bool ColorShaderClass::CreateWVPConstBuffer(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//��� ��� ���� description�� �ۼ�
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//��� ���� �����͸� ������ �ش� Ŭ�������� ���̴� ��� ���ۿ� ������ �� �ְ� �Ѵ�.
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
	if (FAILED(result))
	{
		cout << "����, ��, �������� ��� ���� ����\n";
		return false;
	}

	return true;
}

void ColorShaderClass::ShutDownShader()
{
	//��� ���� ����
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}

	//���̾ƿ� ����
	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}

	//�ȼ� ���̴� ����
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	//���� ���̴� ����
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)errorMessage->GetBufferPointer();
	bufferSize = errorMessage->GetBufferSize();

	fout.open("Shader-Error.txt");
	for (i = 0; i < bufferSize; i++)
		fout << compileErrors[i];
	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox(hwnd, L"Error compiling shader, Check shader-error.txt for message", shaderFilename, MB_OK);
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//����� tranpos�Ͽ� ���̴����� ����� �� �ְ� �Ѵ�.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	//��� ������ ������ �� �� �ֵ��� ���
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	//��� ������ �����Ϳ� ���� �����͸� ������
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//��� ���ۿ� ����� ����
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	//��� ������ ��� ����
	deviceContext->Unmap(matrixBuffer, 0);

	//���� ���̴������� ��� ������ ��ġ ����
	bufferNumber = 0;

	//���� ���̴��� ��� ���۸� �ٲ� ������ ��ü
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//�Է� �ܰ迡 ���̾ƿ� ���ε�
	deviceContext->IASetInputLayout(layout);

	//����, �ȼ� ���̴� ����
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	//indices�� �ﰢ���� �׸�
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
