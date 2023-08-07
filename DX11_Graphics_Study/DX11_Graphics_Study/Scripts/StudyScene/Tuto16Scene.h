#pragma once

#include "../Graphics/GraphicsClass.h"
#include "../Model/ModelClass.h"
#include "../Model/ModelListClass.h"
//#include "../Shader/Class/ColorShaderClass.h"
#include "../Shader/Class/TextureShaderClass.h"
#include "../Shader/Class/MultiTextureShaderClass.h"

#include "../Graphics/LightClass.h"
#include "../Graphics/BitmapClass.h"
#include "../Graphics/TextClass.h"
#include "../Shader/Class/LightShaderClass.h"
#include "../Culling/FrustumClass.h"

class Tuto16Scene : public GraphicsClass
{
private:
	//ColorShaderClass* colorShader;
	//TextureShaderClass* textureClass;
	//ModelClass* model;
	ModelListClass* modelList;
	LightShaderClass* shaderClass;
	LightClass* light;
	TextClass* text;

	FrustumClass* frustum;

	//BitmapClass* bitmap;


public:
	Tuto16Scene();
	Tuto16Scene(const Tuto16Scene& other);
	~Tuto16Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown() override;
	//bool Frame(int mouseX, int mouseY);
	bool Frame(int fps, int cpuValue, float frameTime) override;
	bool Frame(float rotationX, float rotationY) override;
	bool Render(float rotation = 0.0f) override;
};

