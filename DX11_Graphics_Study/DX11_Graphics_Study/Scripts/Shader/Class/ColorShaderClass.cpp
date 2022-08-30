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

	//사용이 끝난 버퍼 해제
	vertexShaderBlob->Release();
	pixelShaderBlob->Release();

	vertexShaderBlob = nullptr;
	pixelShaderBlob = nullptr;

	if (!CreateWVPConstBuffer(device))
		return false;

	return true;
}

//셰이더 컴파일 및 생성
bool ColorShaderClass::CreateVSnPS(ID3D11Device* device, HWND hwnd, const WCHAR* shaderFilename, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	errorMessage = nullptr;

	//정점 셰이더를 컴파일
	result = D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &(*BSBlob), &errorMessage);
	if (FAILED(result))
	{
		cout << "정점 셰이더 컴파일 실패\n";

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

	//픽셀 셰이더 컴파일
	result = D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &(*PSBlob), &errorMessage);
	if (FAILED(result))
	{
		cout << "픽셀 셰이더 컴파일 실패\n";

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

	//버퍼로부터 정점 셰이더를 생성
	result = device->CreateVertexShader((*BSBlob)->GetBufferPointer(), (*BSBlob)->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(result))
		return false;

	//버퍼로부터 픽셀 셰이더를 생성
	result = device->CreatePixelShader((*PSBlob)->GetBufferPointer(), (*PSBlob)->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(result))
		return false;

	return true;
}

//레이아웃 생성
bool ColorShaderClass::CreateLayout(ID3D11Device* device, ID3D10Blob** BSBlob, ID3D10Blob** PSBlob)
{
	HRESULT result;

	unsigned const int numElements = 2;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[numElements];

	//위치 데이터 설정
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	//컬러 데이터 설정
	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//레이아웃 생성
	result = device->CreateInputLayout(polygonLayout, numElements, (*BSBlob)->GetBufferPointer(), (*BSBlob)->GetBufferSize(), &layout);
	if (FAILED(result))
	{
		cout << "레이아웃 생성 실패\n";
		return false;
	}

	return true;
}

//월드, 뷰, 프로젝션 상수 버퍼 생성
bool ColorShaderClass::CreateWVPConstBuffer(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//행렬 상수 버퍼 description를 작성
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//상수 버퍼 포인터를 생성해 해당 클래스에서 셰이더 상수 버퍼에 접근할 수 있게 한다.
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
	if (FAILED(result))
	{
		cout << "월드, 뷰, 프로젝션 행렬 생성 실패\n";
		return false;
	}

	return true;
}

void ColorShaderClass::ShutDownShader()
{
	//상수 버퍼 해제
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}

	//레이아웃 해제
	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}

	//픽셀 셰이더 해제
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	//정점 셰이더 해제
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

	//행렬을 tranpos하여 셰이더에서 사용할 수 있게 한다.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	//상수 버퍼의 내용을 쓸 수 있도록 잠금
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	//상수 버퍼의 데이터에 대한 포인터를 가져옴
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//상수 버퍼에 행렬을 복사
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	//상수 버퍼의 잠금 해제
	deviceContext->Unmap(matrixBuffer, 0);

	//정점 셰이더에서의 상수 버퍼의 위치 설정
	bufferNumber = 0;

	//정점 셰이더의 상수 버퍼를 바뀐 값으로 교체
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//입력 단계에 레이아웃 바인드
	deviceContext->IASetInputLayout(layout);

	//정점, 픽셀 셰이더 설정
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	//indices로 삼각형을 그림
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
