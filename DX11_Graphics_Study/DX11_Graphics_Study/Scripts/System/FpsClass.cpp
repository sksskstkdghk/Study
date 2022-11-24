#include "../DxDefine.h"
#include "FpsClass.h"

FpsClass::FpsClass()
{

}

FpsClass::FpsClass(const FpsClass& other)
{
}

FpsClass::~FpsClass()
{
}

void FpsClass::Init()
{
    fps = count = 0;
    startTime = timeGetTime();
}

void FpsClass::Frame()
{
    count++;

    if (timeGetTime() >= startTime + 1000)
    {
        fps = count;
        count = 0;

        startTime = timeGetTime();
    }
}

int FpsClass::GetFps()
{
    return fps;
}
