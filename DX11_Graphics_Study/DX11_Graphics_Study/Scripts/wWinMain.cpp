#include "DxDefine.h"
#include "System/SystemClass.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	SystemClass* system = new SystemClass();

	//객체 생성
	if (!system)
	{
		cout << "시스템이 존재하지 않습니다.\n";
		return -1;
	}

	//객체 초기화 및 실행
	cout << "시스템 초기화 시작\n";
	if (system->Init())
	{
		cout << "시스템 실행\n";
		system->Run();
	}
	else
	{
		cout << "시스템 초기화 실패.\n";
		return -1;
	}

	//객체 종료 및 메모리 반환
	system->ShutDown();
	delete system;
	system = nullptr;

	return 0;
}