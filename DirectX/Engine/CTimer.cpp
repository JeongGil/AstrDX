#include "CTimer.h"

float CTimer::Update()
{
	prevTime = currTime;
	currTime = std::chrono::steady_clock::now();

	return deltaTime = std::chrono::duration<float>(currTime - prevTime).count();
}
