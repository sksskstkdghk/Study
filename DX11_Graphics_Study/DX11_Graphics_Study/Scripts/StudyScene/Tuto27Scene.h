#pragma once

#include "../Graphics/GraphicsClass.h"
#include "../Shader/Class/TextureShaderClass.h"
#include "../Shader/Class/ReflectionShaderClass.h"
#include "../Model/Texture/RenderTextureClass.h"

class Tuto27Scene : public GraphicsClass
{
private:
	RenderTextureClass* RTT;
	ReflectionShaderClass* reflectionShaderClass;
	ModelClass* floorModel;
	TextureShaderClass* textureShaderClass;
	float rotation;

	bool RenderToTexture();
	bool RenderScene();

public:
	Tuto27Scene();
	Tuto27Scene(const Tuto27Scene&);
	~Tuto27Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown() override;
	bool Render() override;
};

