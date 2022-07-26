#include "../DxDefine.h"
#include "GraphicsClass.h"

bool GraphicsClass::Render()
{
	return true;
}

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	return true;
}

void GraphicsClass::ShutDown()
{
}

bool GraphicsClass::Frame()
{
	return true;
}
