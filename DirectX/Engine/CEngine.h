#pragma once

#include "EngineInfo.h"

class CEngine
{
public:
	bool Init(HINSTANCE hInstance, const TCHAR* WindowName, int IconID, int SmallIconID, int Width, int Height, bool WindowMode);
	int Run();

private:
	void Loop();

	void Update(float deltaTime);
	void Render();

private:
	void InitRegisterClass(const TCHAR* WindowName, int IconID, int SmallIconID) const;
	bool InitCreateWindow(const TCHAR* WindowName, int Width, int Height);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInst = nullptr;
	HWND hWnd = nullptr;

	inline static bool bLoop = true;

public:
	static CEngine* GetInst()
	{
		if (inst == nullptr)
		{
			inst = new CEngine;
		}

		return inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(inst)
	}

private:
	CEngine();
	~CEngine();

	inline static CEngine* inst = nullptr;
};
