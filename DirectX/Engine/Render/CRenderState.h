#pragma once

#include "../EngineInfo.h"

class CRenderState
{
	friend class CRenderManager;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;

protected:
	CRenderState() = default;

public:
	virtual  ~CRenderState();

	ID3D11DeviceChild* State = nullptr;
	ID3D11DeviceChild* PrevState = nullptr;

private:
	unsigned int StencilRef = 0xFFFFFFFF;
	unsigned int PrevStencilRef = 0xFFFFFFFF;
};

