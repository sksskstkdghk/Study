#pragma once
#include <mmsystem.h>

class FpsClass
{
private:
	int fps, count;
	unsigned long startTime;

public:
	FpsClass();
	FpsClass(const FpsClass& other);
	~FpsClass();

	void Init();
	void Frame();
	int GetFps();
};

