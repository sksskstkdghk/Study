#pragma once

#include <dinput.h>

class InputClass
{
private:
	//bool keys[256] = {};
	IDirectInput8* directInput;
	IDirectInputDevice8* keyboard, *mouse;

	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;

	int screenWidth, screenHeight;
	int mouseX, mouseY;

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	bool SetKeyboard(HWND hwnd);
	bool SetMouse(HWND hwnd);

public:
	InputClass();
	InputClass(const InputClass& other);
	~InputClass();

	bool Init(HINSTANCE instance, HWND hwnd, int screenWidth, int screenHeight);
	void ShutDown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

	/*void KeyDown(unsigned int index);
	void KeyUp(unsigned int index);*/
	
	bool isKeyDown(unsigned int index);
};

