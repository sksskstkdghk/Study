#pragma once

#include "D3DClass.h"
#include "CameraClass.h"
#include "../Model/ModelClass.h"
#include "../Shader/Class/MultiTextureShaderClass.h"


const bool FULL_SCREEN = false;		//전체화면 여부
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;	//최대 보여지는 z축 거리
const float SCREEN_NEAR = 0.1f;		//최소 보여지는 z축 거리

class GraphicsClass
{
protected:
	D3DClass* D3D;
	CameraClass* camera;

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
private:
	ModelClass* model;
	MultiTextureShaderClass* shaderClass;

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	bool virtual Init(int screenWidth, int screenHeight, HWND hwnd);
	void virtual ShutDown();
	//bool Frame(int mouseX, int mouseY);
	bool virtual Frame(int fps, int cpuValue, float frameTime);
	bool virtual Frame(float rotationX, float rotationY);
	bool virtual Render(float rotation = 0.0f);
};

