#pragma once
class TimerClass
{
private:
	INT64 frequency, startTime;
	float ticksPerMs, frameTime;


public:
	TimerClass();
	TimerClass(const TimerClass& other);
	~TimerClass();

	bool Init();
	void Frame();
	float GetTime();
};

