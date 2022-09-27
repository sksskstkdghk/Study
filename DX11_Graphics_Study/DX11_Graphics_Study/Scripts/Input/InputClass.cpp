#include "../DxDefine.h"
#include "InputClass.h"

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			mouse->Acquire();
		else
			return false;
	}

	return true;
}

void InputClass::ProcessInput()
{
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;

	if (mouseX < 0) mouseX = 0;
	if (mouseY < 0) mouseY = 0;

	if (mouseX > screenWidth) mouseX = screenWidth;
	if (mouseY > screenHeight) mouseY = screenHeight;
}
//키보드 세팅
bool InputClass::SetKeyboard(HWND hwnd)
{
	HRESULT result;

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	if (FAILED(result))
		return false;

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	//키보드 협력레벨(무엇을 하고 어떻게 사용될 것인지) 설정
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	//키보드 가져오기
	result = keyboard->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

//마우스 세팅
bool InputClass::SetMouse(HWND hwnd)
{
	HRESULT result;

	result = directInput->CreateDevice(GUID_SysMouse, &mouse, nullptr);
	if (FAILED(result))
		return false;

	result = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	result = mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	result = mouse->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

InputClass::InputClass()
{
	directInput = nullptr;
	keyboard = mouse = nullptr;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::ShutDown()
{
	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = nullptr;
	}

	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = nullptr;
	}

	if (directInput)
	{
		directInput->Release();
		directInput = nullptr;
	}
}

bool InputClass::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
		return false;

	result = ReadMouse();
	if (!result)
		return false;

	ProcessInput();

	return true;
}

bool InputClass::IsEscapePressed()
{
	if (keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = this->mouseX;
	mouseY = this->mouseY;
}

bool InputClass::Init(HINSTANCE instance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	mouseX = mouseY = 0;

	result = DirectInput8Create(instance, 0x0800, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(result))
		return false;

	if (!SetKeyboard(hwnd))
		return false;

	if (!SetMouse(hwnd))
		return false;

	return true;
}

////키가 눌렸다면 true할당
//void InputClass::KeyDown(unsigned int index)
//{
//	keys[index] = true;
//}
//
////키가 해제되면 false할당
//void InputClass::KeyUp(unsigned int index)
//{
//	keys[index] = false;
//}
//
////현재 키 값이 눌렸는가?
//bool InputClass::isKeyDown(unsigned int index)
//{
//	return keys[index];
//}
