#pragma once

#include "D3DClass.h"
#include "CameraClass.h"
#include "../Model/ModelClass.h"
//#include "../Shader/Class/ColorShaderClass.h"
#include "../Shader/Class/TextureShaderClass.h"

#include "LightClass.h"
#include "BitmapClass.h"
#include "TextClass.h"
#include "../Shader/Class/LightShaderClass.h"

const bool FULL_SCREEN = true;		//전체화면 여부
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;	//최대 보여지는 z축 거리
const float SCREEN_NEAR = 0.1f;		//최소 보여지는 z축 거리

class GraphicsClass
{
private:
	D3DClass* D3D;
	CameraClass* camera;
	//ModelClass* model;
	//ColorShaderClass* colorShader;
	//TextureShaderClass* textureClass;
	//LightShaderClass* shaderClass;
	//LightClass* light;
	TextClass* text;

	//BitmapClass* bitmap;

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	bool Init(int screenWidth, int screenHeight, HWND hwnd);
	void ShutDown();
	bool Frame(int mouseX, int mouseY);
	bool Render(float rotation = 0.0f);
};

