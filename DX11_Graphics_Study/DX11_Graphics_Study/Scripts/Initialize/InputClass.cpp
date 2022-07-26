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
	//Ű �迭 �ʱ�ȭ
	for (int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}

	return true;
}

//Ű�� ���ȴٸ� true�Ҵ�
void InputClass::KeyDown(unsigned int index)
{
	keys[index] = true;
}

//Ű�� �����Ǹ� false�Ҵ�
void InputClass::KeyUp(unsigned int index)
{
	keys[index] = false;
}

//���� Ű ���� ���ȴ°�?
bool InputClass::isKeyDown(unsigned int index)
{
	return keys[index];
}
