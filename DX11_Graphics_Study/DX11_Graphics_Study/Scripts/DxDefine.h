#pragma once
//#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "d3dx10.lib")
//#pragma comment(lib, "d3dx9.lib")

//�̸� �����ϵ� ������� ���
//�̱���, dxapi���� ���� �������� ������ ���

#define WIN32_LEAN_AND_MEAN	//���� ������ �ʴ� ������ ������� �ڵ� ����
#define DIRECTINPUT_VERSION 0x0800	//dxInput����

#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <vector>

#define Win_Width	0
#define Win_Height	0

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3d9types.h>
#include <d3dcompiler.h>

using namespace std;
using namespace DirectX;

typedef XMVECTOR XMPLANE;
typedef XMFLOAT4 XMCOLOR;

//Initialize
//#include "Initialize/GraphicsClass.h"
//#include "Initialize/InputClass.h"