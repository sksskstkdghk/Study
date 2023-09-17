#pragma once

#include "../Graphics/GraphicsClass.h"
#include "../Shader/Class/FogShaderClass.h"

class Tuto23Scene : public GraphicsClass
{
private:
	FogShaderClass* fogShaderClass;
	float rotation;

public:
	Tuto23Scene();
	Tuto23Scene(const Tuto23Scene&);
	~Tuto23Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown() override;
	bool Render() override;
};

