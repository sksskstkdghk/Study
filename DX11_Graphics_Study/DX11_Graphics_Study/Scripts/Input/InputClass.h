#pragma once

#include <dinput.h>

class InputClass
{
private:
	bool keys[256] = {};


public:
	InputClass();
	InputClass(const InputClass& other);
	~InputClass();

	bool Init();

	void KeyDown(unsigned int index);
	void KeyUp(unsigned int index);

	bool isKeyDown(unsigned int index);
};

