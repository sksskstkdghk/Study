#include "../DxDefine.h"
#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Init()
{
	//키 배열 초기화
	for (int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}

	return true;
}

//키가 눌렸다면 true할당
void InputClass::KeyDown(unsigned int index)
{
	keys[index] = true;
}

//키가 해제되면 false할당
void InputClass::KeyUp(unsigned int index)
{
	keys[index] = false;
}

//현재 키 값이 눌렸는가?
bool InputClass::isKeyDown(unsigned int index)
{
	return keys[index];
}
