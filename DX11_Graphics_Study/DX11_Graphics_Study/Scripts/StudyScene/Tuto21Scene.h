#pragma once

#include "../Graphics/GraphicsClass.h"
#include "../Graphics/LightClass.h"
#include "../Shader/Class/SpecMapShaderClass.h"

class Tuto21Scene : public GraphicsClass
{
private:
	SpecMapShaderClass* specMapShader;
	LightClass* light;

	float rotation = 0.0f;

public:
	Tuto21Scene();
	Tuto21Scene(const Tuto21Scene& other);
	~Tuto21Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown();
	bool Frame();
	bool Render() override;
};
