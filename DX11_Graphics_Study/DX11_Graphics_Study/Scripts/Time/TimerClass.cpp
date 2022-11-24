#include "../DxDefine.h"
#include "TimerClass.h"

TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass& other)
{
}

TimerClass::~TimerClass()
{
}

bool TimerClass::Init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
		return false;

	ticksPerMs = (float)(frequency / 1000);
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - startTime);

	frameTime = timeDifference / ticksPerMs;
	startTime = currentTime;
}

float TimerClass::GetTime()
{
	return frameTime;
}
