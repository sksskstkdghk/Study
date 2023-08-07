#pragma once

#include "../Graphics/GraphicsClass.h"

class Tuto17Scene : public GraphicsClass
{
private:
	//ModelClass* model;
	MultiTextureShaderClass* shaderClass;

public:
	Tuto17Scene();
	Tuto17Scene(const Tuto17Scene& other);
	~Tuto17Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown() override;
	bool Render(float rotation = 0.0f) override;

	//필요 시 사용
	//bool Frame(int fps, int cpuValue, float frameTime) override;
	//bool Frame(float rotationX, float rotationY) override;
};

