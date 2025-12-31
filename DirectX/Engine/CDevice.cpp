#include "CDevice.h"

bool CDevice::Init(const HWND window, const int width, const int height, const bool windowMode)
{
	this->window = window;
	Resolution.Width = width;
	Resolution.Height = height;
	bWindowMode = windowMode;
	
	UINT flag{ 0u };
#ifdef _DEBUG
	flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL featureLevelResult;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag, &featureLevel, 1,
		D3D11_SDK_VERSION, &Device, &featureLevelResult, &Context)))
	{
		return false;
	}

	//UINT sampleCount{ 4u };
	//UINT quality{ 0u };
	//UINT maxQuality{ 0u };
	//if (SUCCEEDED(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &quality)))
	//{
	//	if (quality < 1u)
	//	{
	//		sampleCount = 1u;
	//		quality = 0u;
	//	}
	//	else
	//	{
	//		maxQuality = quality - 1;
	//	}
	//}

	//sampleCount = 1u;

	//// FLIP_DISCARD only supports quality 0.
	//quality = 0u;

	//DEVMODE devMode{ .dmSize = sizeof(DEVMODE) };
	//if (!EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode))
	//{
	//	devMode.dmDisplayFrequency = 60ul;
	//}

	DXGI_SWAP_CHAIN_DESC swapDesc
	{
		.BufferDesc
		{
			.Width = static_cast<UINT>(width),
			.Height = static_cast<UINT>(height),
			.RefreshRate
			{
				.Numerator = 1u,

				// OS may handle the refresh rate when using FLIP_DISCARD.
				.Denominator = 0,
			},
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			.Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
		},
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0,
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		// more than 2 buffers for DXGI_SWAP_EFFECT_FLIP_DISCARD
		.BufferCount = 2u,
		.OutputWindow = window,
		.Windowed = windowMode,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.Flags = 0u,
	};

	IDXGIDevice* DXGIDevice = nullptr;
	if (FAILED(Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice)))
	{
		return false;
	}

	IDXGIAdapter* adapter = nullptr;
	if (FAILED(DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter)))
	{
		DXGIDevice->Release();

		return false;
	}

	IDXGIFactory* factory = nullptr;
	if (FAILED(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory)))
	{
		adapter->Release();
		DXGIDevice->Release();

		return false;
	}

	if (FAILED(factory->CreateSwapChain(Device, &swapDesc, &SwapChain)))
	{
		factory->Release();
		adapter->Release();
		DXGIDevice->Release();

		return false;
	}

	factory->Release();
	adapter->Release();
	DXGIDevice->Release();

	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(SwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		return false;
	}

	if (FAILED(Device->CreateRenderTargetView(backBuffer, nullptr, &TargetView)))
	{
		backBuffer->Release();

		return false;
	}
	
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC depthDesc
	{
		.Width = static_cast<UINT>(width),
		.Height = static_cast<UINT>(height),
		.MipLevels = 1u,
		.ArraySize = 1u,
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc
		{
			.Count = 1u,
			.Quality = 0u,
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
	};

	ID3D11Texture2D* depthBuffer = nullptr;
	if (FAILED(Device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer)))
	{
		return false;
	}

	if (FAILED( Device->CreateDepthStencilView(depthBuffer, nullptr, &DepthView)))
	{
		depthBuffer->Release();

		return false;
	}

	depthBuffer->Release();

	D3D11_VIEWPORT vp
	{
		.TopLeftX = 0.f,
		.TopLeftY = 0.f,
		.Width = static_cast<float>(width),
		.Height = static_cast<float>(height),
		.MinDepth = 0.f,
		.MaxDepth = 1.f,
	};

	Context->RSSetViewports(1, &vp);

	return true;
}

void CDevice::ClearBackBuffer()
{
	constexpr float color[4] = {};
	Context->ClearRenderTargetView(TargetView, color);
}

void CDevice::ClearDepthStencil(const float depth, const UINT8 stencil)
{
	Context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void CDevice::SetTarget()
{
	Context->OMSetRenderTargets(1, &TargetView, DepthView);
}

void CDevice::BeginRender()
{
	ClearBackBuffer();
	ClearDepthStencil();
	SetTarget();
}

void CDevice::EndRender()
{
	SwapChain->Present(0u, 0u);
}

CDevice::CDevice()
{
}

CDevice::~CDevice()
{
	if (TargetView)
	{
		TargetView->Release();
	}

	if (DepthView)
	{
		DepthView->Release();
	}

	if (SwapChain)
	{
		SwapChain->Release();
	}

	if (Context)
	{
		Context->ClearState();
		Context->Release();
	}

	if (Device)
	{
		Device->Release();
	}
}
