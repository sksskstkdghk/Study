#include "CpuClass.h"
#include "FpsClass.h"
#include "../Time/TimerClass.h"
#include "PositionClass.h"


class InputClass;
class GraphicsClass;
class SoundClass;
class PositionClass;

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

	//초기화
	bool Init();
	//실행
	void Run();
	//종료
	void ShutDown();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static SystemClass* AppHandle = 0;