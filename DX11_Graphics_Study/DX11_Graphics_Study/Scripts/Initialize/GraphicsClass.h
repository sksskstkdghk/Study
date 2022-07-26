#pragma once

const bool FULL_SCREEN = false;		//��üȭ�� ����
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;	//�ִ� �������� z�� �Ÿ�
const float SCREEN_NEAR = 0.1f;		//�ּ� �������� z�� �Ÿ�

class GraphicsClass
{
private:
	bool Render();

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	bool Init(int screenWidth, int screenHeight, HWND hwnd);
	void ShutDown();
	bool Frame();

};

