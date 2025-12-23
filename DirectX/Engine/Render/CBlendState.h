#pragma once
#include "CRenderState.h"

class CBlendState :
	public CRenderState
{
	friend class CRenderManager;

public:
	void SetState() override;
	void ResetState() override;

	void AddRenderTargetDesc(bool BlendEnable = true,
		D3D11_BLEND SrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND DestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	void SetBlendFactor(float r, float g, float b, float a);
	void SetSampleMask(UINT SampleMask);

	// AlphaToCoverageEnable: When using MSAA, it converts the alpha value into
	// a sample mask to smooth transparent edges. This is often used for objects like leaves and grass.
	//
	// IndependentBlendEnable: If set to false, the value of RenderTarget[0] is replicated to targets 1 through 7.
	// If set to true, each target can be specified independently.
	bool CreateState(bool AlphaToCoverageEnable = false, bool IndependentBlendEnable = false);

protected:
	CBlendState() = default;

public:
	~CBlendState() override = default;

private:
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC> TargetDescs;
	float BlendFactor[4] = { 1.f,1.f, 1.f, 1.f, };
	float PrevBlendFactor[4] = {};
	UINT SampleMask = 0xFFFFFFFF;
	UINT PrevSampleMask = 0xFFFFFFFF;
};

