#pragma once

#include "EngineInfo.h"

class CDevice
{
public:
	bool Init(HWND window, const int width, const int height, const bool windowMode);
	void ClearBackBuffer();
	void ClearDepthStencil(float depth = 1.f, UINT8 stencil = 0);
	void SetTarget();
	void BeginRender();
	void EndRender();

private:
	// It is mainly used when creating resources.
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* targetView = nullptr;
	ID3D11DepthStencilView* depthView = nullptr;

	FResolution resolution;

	HWND window = nullptr;

public:
	static CDevice* GetInst()
	{
		if (inst == nullptr)
		{
			inst = new CDevice;
		}

		return inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(inst)
	}

private:
	CDevice();
	~CDevice();

	inline static CDevice* inst = nullptr;
};

