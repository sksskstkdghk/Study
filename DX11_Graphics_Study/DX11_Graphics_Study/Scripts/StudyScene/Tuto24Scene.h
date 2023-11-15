#pragma once

#include "../Graphics/GraphicsClass.h"
#include "../Shader/Class/ClipplaneShaderClass.h"


class Tuto24Scene : public GraphicsClass
{
private:
	float rotation;

	ClipplaneShaderClass* clipplaneShaderClass;
	XMFLOAT4 clipPlane;

public:
	Tuto24Scene();
	Tuto24Scene(const Tuto24Scene&);
	~Tuto24Scene();

	bool Init(int screenWidth, int screenHeight, HWND hwnd) override;
	void ShutDown() override;
	bool Render() override;
};

