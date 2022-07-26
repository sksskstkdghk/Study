#pragma once
//#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

//미리 컴파일된 헤더파일 장소
//싱글톤, dxapi내부 변수 전역선언 등으로 사용

#define WIN32_LEAN_AND_MEAN	//거의 사용되지 않는 내용은 헤더에서 자동 제거

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