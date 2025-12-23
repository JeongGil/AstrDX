#include "CDepthStencilState.h"

#include "../CDevice.h"

void CDepthStencilState::SetState()
{
	CDevice::GetInst()->GetContext()->OMGetDepthStencilState((ID3D11DepthStencilState**)&PrevState, &PrevStencilRef);
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)State, StencilRef);
}

void CDepthStencilState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)PrevState, PrevStencilRef);

	SAFE_RELEASE(PrevState);
}

bool CDepthStencilState::CreateState(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask,
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, UINT8 StencilReadMask, UINT8 StencilWriteMask,
	D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	D3D11_DEPTH_STENCIL_DESC Desc =
	{
		.DepthEnable = DepthEnable,
		.DepthWriteMask = DepthWriteMask,
		.DepthFunc = DepthFunc,
		.StencilEnable = StencilEnable,
		.StencilReadMask = StencilReadMask,
		.StencilWriteMask = StencilWriteMask,
		.FrontFace = FrontFace,
		.BackFace = BackFace
	};

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&Desc, (ID3D11DepthStencilState**)&State)))
	{
		return false;
	}

	return true;
}
