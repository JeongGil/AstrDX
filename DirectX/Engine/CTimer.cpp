#include "CTimer.h"

float CTimer::Update(HWND hWnd)
{
	PrevTime = CurrTime;
	CurrTime = std::chrono::steady_clock::now();

	DeltaTime = std::chrono::duration<float>(CurrTime - PrevTime).count();

	Elapsed += DeltaTime;
	if (++TickCounter == 60)
	{
		FPS = static_cast<float>(TickCounter) / Elapsed;
		Elapsed = 0.f;
		TickCounter = 0;

		char FPSText[64] = {};

		sprintf_s(FPSText, "FPS : %.5f\n", FPS);

#ifdef _DEBUG
		OutputDebugStringA(FPSText);
#else
		SetWindowTextA(hWnd, FPSText);
#endif
	}

	return DeltaTime;
}
