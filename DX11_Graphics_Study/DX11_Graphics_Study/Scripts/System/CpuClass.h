#pragma once
#pragma comment(lib, "pdh.lib")
#include <Pdh.h>

class CpuClass
{
private:
	bool canReadCpu;
	HQUERY queryHandle;
	HCOUNTER counterHandle;
	unsigned long lastSampleTime;
	long cpuUsage;

public:
	CpuClass();
	CpuClass(const CpuClass& other);
	~CpuClass();

	void Init();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();
};

