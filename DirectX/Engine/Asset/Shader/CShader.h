#pragma once
#include "../CAsset.h"

class CShader :
	public CAsset
{
	friend class CShaderManager;

protected:
	bool bUseRenderInstancing = true;

public:
	virtual bool Init(const std::string& PathName) = 0;
	virtual void SetShader() = 0;

	[[nodiscard]] bool IsUseRenderInstancing() const
	{
		return bUseRenderInstancing;
	}

	void SetUseRenderInstancing(const bool bUse)
	{
		bUseRenderInstancing = bUse;
	}

protected:
	CShader();

public:
	virtual ~CShader();
};

