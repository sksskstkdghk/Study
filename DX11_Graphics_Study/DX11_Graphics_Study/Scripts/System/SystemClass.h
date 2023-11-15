#include "CpuClass.h"
#include "FpsClass.h"
#include "../Time/TimerClass.h"
#include "PositionClass.h"

#pragma region �� ��Ŭ��� ����
//#include "Scripts/StudyScene/Tuto17Scene.h"
//#include "Scripts/StudyScene/Tuto20Scene.h"
//#include "Scripts/StudyScene/Tuto21Scene.h"
//#include "Scripts/StudyScene/Tuto22Scene.h"
//#include "Scripts/StudyScene/Tuto23Scene.h"
//#include "Scripts/StudyScene/Tuto24Scene.h"
#include "Scripts/StudyScene/Tuto27Scene.h"
#pragma endregion



#pragma region ���漱��

class InputClass;
class GraphicsClass;
class SoundClass;
class PositionClass;

#pragma endregion

class SystemClass
{
private:
	LPCWSTR appName;
	HINSTANCE hInstance;
	HWND hwnd;

	InputClass* input = nullptr;
	GraphicsClass* graphic = nullptr;
	SoundClass* sound = nullptr;

	FpsClass* fps;
	CpuClass* cpuClass;
	TimerClass* timer;
	PositionClass* position;

	bool Frame();
	void InitWindows(int& Width, int& Height);
	void ShutDownWindows();

public:
	SystemClass();
	SystemClass(const SystemClass& other);
	~SystemClass();

	//�ʱ�ȭ
	bool Init();
	//����
	void Run();
	//����
	void ShutDown();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

};

//������ ���μ��� �Լ�
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static SystemClass* AppHandle = 0;