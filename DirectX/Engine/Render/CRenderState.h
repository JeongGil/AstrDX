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

protected:
	ID3D11DeviceChild* State = nullptr;
	ID3D11DeviceChild* PrevState = nullptr;

	std::string Key;

private:
	unsigned int StencilRef = 0xFFFFFFFF;
	unsigned int PrevStencilRef = 0xFFFFFFFF;

public:
	[[nodiscard]] std::string GetKey() const
	{
		return Key;
	}

	void SetKey(const std::string& Key)
	{
		this->Key = Key;
	}
};

