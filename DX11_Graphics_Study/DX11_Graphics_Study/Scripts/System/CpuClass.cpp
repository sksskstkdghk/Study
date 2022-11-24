#include "CpuClass.h"

CpuClass::CpuClass()
{

}

CpuClass::CpuClass(const CpuClass& other)
{
}

CpuClass::~CpuClass()
{
}

void CpuClass::Init()
{
    PDH_STATUS status;

    canReadCpu = true;

    //���� ������ �÷����� �����ϴ� query����(cpu������)
    status = PdhOpenQuery(nullptr, 0, &queryHandle);
    if (status != ERROR_SUCCESS)
        canReadCpu = false;

    //query�� ������ ī���� �߰�
    status = PdhAddCounter(queryHandle, TEXT("\\Processor(Total)\\% processor time"), 0, &counterHandle);
    if (status != ERROR_SUCCESS)
        canReadCpu = false;

    lastSampleTime = GetTickCount();

    cpuUsage = 0;
}

void CpuClass::Shutdown()
{
    if (canReadCpu)
    {
        PdhCloseQuery(queryHandle);
    }
}

void CpuClass::Frame()
{
    PDH_FMT_COUNTERVALUE value;

    if (canReadCpu)
    {
        if (lastSampleTime + 1000 < GetTickCount())
        {
            lastSampleTime = GetTickCount();
            PdhCollectQueryData(queryHandle);
            PdhGetFormattedCounterValue(counterHandle, PDH_FMT_LONG, NULL, &value);
            cpuUsage = value.longValue;
        }
    }
}

int CpuClass::GetCpuPercentage()
{
    return (canReadCpu) ? (int)cpuUsage : 0;
}
