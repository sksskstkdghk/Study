
class InputClass;
class GraphicsClass;

class SystemClass
{
private:
	LPCWSTR appName;
	HINSTANCE hInstance;
	HWND hwnd;

	InputClass* input = nullptr;
	GraphicsClass* graphic = nullptr;

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

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static SystemClass* AppHandle = 0;