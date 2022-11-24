#include "../DxDefine.h"
#include "PositionClass.h"

PositionClass::PositionClass()
{
	frameTime = rotationY = rotationX = leftTurnSpeed = rightTurnSpeed = UpTurnSpeed = DownTurnSpeed = 0.0f;
}

PositionClass::PositionClass(const PositionClass& other)
{
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float frameTime)
{
	this->frameTime = frameTime;
}

void PositionClass::GetRotation(float& rotationX, float& rotationY)
{
	rotationX = this->rotationX;
	rotationY = this->rotationY;
}

void PositionClass::TurnLeft(bool keydown)
{
	if (keydown)
	{
		leftTurnSpeed += frameTime * 0.01f;

		if (leftTurnSpeed > frameTime * 0.1f)
			leftTurnSpeed = frameTime * 0.1f;

		rotationY -= leftTurnSpeed;
		if (rotationY < 0.0f)
			rotationY += 360.0f;
	}
}

void PositionClass::TurnRight(bool keydown)
{
	if (keydown)
	{
		rightTurnSpeed += frameTime * 0.01f;

		if (rightTurnSpeed > frameTime * 0.1f)
			rightTurnSpeed = frameTime * 0.1f;

		rotationY += leftTurnSpeed;

		if (rotationY > 360.0f)
			rotationY -= 360.0f;
	}
}

void PositionClass::TurnUp(bool keydown)
{
	if (keydown)
	{
		UpTurnSpeed += frameTime * 0.01f;

		if (UpTurnSpeed > frameTime * 0.1f)
			UpTurnSpeed = frameTime * 0.1f;

		rotationX -= UpTurnSpeed;
		if (rotationX < 0.0f)
			rotationX += 360.0f;
	}
}

void PositionClass::TurnDown(bool keydown)
{
	if (keydown)
	{
		DownTurnSpeed += frameTime * 0.01f;

		if (DownTurnSpeed > frameTime * 0.1f)
			DownTurnSpeed = frameTime * 0.1f;

		rotationX += DownTurnSpeed;
		if (rotationX > 360.0f)
			rotationX -= 360.0f;
	}
}
