#pragma once
//#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

//�̸� �����ϵ� ������� ���
//�̱���, dxapi���� ���� �������� ������ ���

#define WIN32_LEAN_AND_MEAN	//���� ������ �ʴ� ������ ������� �ڵ� ����

#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <memory.h>
#include <tchar.h>

using namespace std;

#define Win_Width	0
#define Win_Height	0

//Initialize
//#include "Initialize/GraphicsClass.h"
//#include "Initialize/InputClass.h"