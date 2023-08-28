#pragma once

#include "../Graphics/GraphicsClass.h"
#include "../Graphics/LightClass.h"
#include "../Shader/Class/BumpMapShaderClass.h"
#include "../Shader/Class/MultiTextureShaderClass.h"

class Tuto20Scene : public GraphicsClass
{
private:
	BumpMapShaderClass* bumpMapShader;
	//MultiTextureShaderClass* bumpMapShader;
	LightClass* light;

	float rotation = 0.0f;

public:
	Tuto20Scene();
	Tuto20Scene(const Tuto20Scene& other);
	~Tuto20Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown();
	bool Frame();
	bool Render() override;
};

