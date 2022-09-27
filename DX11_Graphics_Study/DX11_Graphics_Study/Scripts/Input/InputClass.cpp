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
//Ű���� ����
bool InputClass::SetKeyboard(HWND hwnd)
{
	HRESULT result;

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	if (FAILED(result))
		return false;

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	//Ű���� ���·���(������ �ϰ� ��� ���� ������) ����
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	//Ű���� ��������
	result = keyboard->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

//���콺 ����
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

////Ű�� ���ȴٸ� true�Ҵ�
//void InputClass::KeyDown(unsigned int index)
//{
//	keys[index] = true;
//}
//
////Ű�� �����Ǹ� false�Ҵ�
//void InputClass::KeyUp(unsigned int index)
//{
//	keys[index] = false;
//}
//
////���� Ű ���� ���ȴ°�?
//bool InputClass::isKeyDown(unsigned int index)
//{
//	return keys[index];
//}
