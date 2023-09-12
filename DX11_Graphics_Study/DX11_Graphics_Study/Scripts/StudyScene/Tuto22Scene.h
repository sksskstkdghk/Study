#pragma once

#include "../Graphics/GraphicsClass.h"
#include "../Graphics/LightClass.h"
#include "../Shader/Class/LightShaderClass.h"
#include "../Model/Texture/RenderTextureClass.h"
#include "../Graphics/DebugWindowClass.h"
#include "../Shader/Class/TextureShaderClass.h"

class Tuto22Scene : public GraphicsClass
{
private:
	LightShaderClass* lightShader;
	LightClass* light;
	RenderTextureClass* renderTexture;
	DebugWindowClass* debugWindow;
	TextureShaderClass* textureShader;

	float rotation = 0.0f;

	bool RenderToTexture();
	bool RenderScene();

public:
	Tuto22Scene();
	Tuto22Scene(const Tuto22Scene& other);
	~Tuto22Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown();
	bool Frame();
	bool Render() override;
};
