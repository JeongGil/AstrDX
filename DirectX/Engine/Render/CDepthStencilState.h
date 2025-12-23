#pragma once
#include "CRenderState.h"

class CDepthStencilState :
	public CRenderState
{
	friend class CRenderManager;

public:
	void SetState() override;
	void ResetState() override;

	// DepthEnable: Determines whether to use depth or not.
	// DepthWriteMask: Consists of ZERO and ALL. 
	// If set to ZERO, depth values are not written to the depth buffer. 
	// If set to ALL, depth values are written.
	// DepthFunc: Specifies the function to compare depth values.
	// StencilEnable: Determines whether to use the stencil buffer.
	// StencilReadMask: The bitmask to read and compare during stencil testing.
	bool CreateState(bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 StencilReadMask = 0xFF,
		UINT8 StencilWriteMask = 0xFF,
		D3D11_DEPTH_STENCILOP_DESC FrontFace =
		{
			.StencilFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilPassOp = D3D11_STENCIL_OP_KEEP,
			.StencilFunc = D3D11_COMPARISON_ALWAYS
		},
		D3D11_DEPTH_STENCILOP_DESC BackFace =
		{
			.StencilFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilPassOp = D3D11_STENCIL_OP_KEEP,
			.StencilFunc = D3D11_COMPARISON_ALWAYS
		});

protected:
	CDepthStencilState() = default;

public:
	~CDepthStencilState() override = default;

private:
	unsigned int StencilRef = 0xFFFFFFFF;
	unsigned int PrevStencilRef = 0xFFFFFFFF;
};

