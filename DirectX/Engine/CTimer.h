#pragma once
#include "EngineInfo.h"

struct FTimerHandle
{
	float RemainTime;
	std::function<void()> Callback;
	std::weak_ptr<void> Owner;
};

class CTimer
{
	inline static std::chrono::steady_clock::time_point PrevTime = std::chrono::steady_clock::now();
	inline static std::chrono::steady_clock::time_point CurrTime = std::chrono::steady_clock::now();
	inline static float DeltaTime = 0.f;

	inline static float FPS{ 0.f };
	inline static float Elapsed{ 0.f };
	inline static int TickCounter{ 0 };

	inline static std::list<FTimerHandle> Timers;

public:
	static float Update(HWND hWnd);

	template <typename T, typename Func, typename Arg>
	static void SetTimer(float InDelay, const std::weak_ptr<T>& InOwner, Func InFunc, Arg InArg)
	{
		FTimerHandle NewHandle;
		NewHandle.RemainTime = InDelay;
		NewHandle.Owner = InOwner;
		NewHandle.Callback = [InOwner, InFunc, InArg]()
			{
				if (auto Owner = InOwner.lock())
				{
					std::invoke(InFunc, Owner.get(), InArg);
				}
			};

		Timers.push_back(NewHandle);
	}

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

