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

    //성능 데이터 컬렉션을 관리하는 query생성(cpu데이터)
    status = PdhOpenQuery(nullptr, 0, &queryHandle);
    if (status != ERROR_SUCCESS)
        canReadCpu = false;

    //query에 지정된 카운터 추가
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
