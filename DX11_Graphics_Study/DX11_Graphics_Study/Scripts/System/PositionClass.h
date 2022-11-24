#pragma once

class PositionClass
{
private:
	float frameTime;
	float rotationY, rotationX;
	float leftTurnSpeed, rightTurnSpeed, UpTurnSpeed, DownTurnSpeed;

public:
	PositionClass();
	PositionClass(const PositionClass& other);
	~PositionClass();

	void SetFrameTime(float frameTime);
	void GetRotation(float& rotationX, float& rotationY);

	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);
	void TurnUp(bool keydown);
	void TurnDown(bool keydown);
};

