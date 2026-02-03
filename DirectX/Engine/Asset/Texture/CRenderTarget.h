#pragma once
#include "CTexture.h"

class CRenderTarget :
    public CTexture
{

protected:
	CRenderTarget();

public:
	~CRenderTarget() override;

private:
	ID3D11Texture2D* Texture{};
	ID3D11RenderTargetView* Target{};
	ID3D11RenderTargetView* PrevTarget{};

	ID3D11Texture2D* DepthTexture{};
	ID3D11DepthStencilView* Depth{};
	ID3D11DepthStencilView* PrevDepth{};

	D3D11_VIEWPORT VP{};
	D3D11_VIEWPORT PrevVP{};
	UINT PrevVPCount{ 1 };
	float ClearColor[4]{};

public:
	static std::shared_ptr<CRenderTarget> Create(const std::string& Key, UINT Width, UINT Height, DXGI_FORMAT Format, bool DepthEnable = false);

	bool Init(UINT Width, UINT Height, DXGI_FORMAT Format, bool DepthEnable = false);
	void SetTarget();
	void ResetTarget();

	void SetClearColor(float r, float g, float b, float a)
	{
		ClearColor[0] = r;
		ClearColor[1] = g;
		ClearColor[2] = b;
		ClearColor[3] = a;
	}
};

