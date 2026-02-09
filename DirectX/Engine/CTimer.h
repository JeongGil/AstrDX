#pragma once
#include "EngineInfo.h"

class CTimer
{
	inline static std::chrono::steady_clock::time_point PrevTime = std::chrono::steady_clock::now();
	inline static std::chrono::steady_clock::time_point CurrTime = std::chrono::steady_clock::now();
	inline static float DeltaTime = 0.f;

	inline static float FPS{ 0.f };
	inline static float Elapsed{ 0.f };
	inline static int TickCounter{ 0 };

public:
	static float Update(HWND hWnd);

	static float GetDeltaTime()
	{
		return DeltaTime;
	}

	static float GetFPS()
	{
		return FPS;
	}

	static std::chrono::steady_clock::time_point Now()
	{
		return CurrTime;
	}
};

