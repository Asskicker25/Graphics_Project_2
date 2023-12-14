#pragma once

class Timer
{
public:
	static Timer& GetInstance();

	float deltaTime{ 0 };
	float currentTime{ 0 };
	float lastFrameTime{ 0 };

	void SetCurrentTime(float currentTime);

};

