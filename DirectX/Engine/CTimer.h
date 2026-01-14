#pragma once
#include <chrono>

class CTimer
{
	inline static std::chrono::steady_clock::time_point prevTime = std::chrono::steady_clock::now();
	inline static std::chrono::steady_clock::time_point currTime = std::chrono::steady_clock::now();
	inline static float deltaTime = 0.f;

public:
	static float Update();

	static float GetDeltaTime()
	{
		return deltaTime;
	}
};

