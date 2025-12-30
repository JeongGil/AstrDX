#pragma once
#include "../CAsset.h"

class CShader :
	public CAsset
{
	friend class CShaderManager;

public:
	virtual bool Init(const std::string& PathName) = 0;
	virtual void SetShader() = 0;

protected:
	CShader();

public:
	virtual ~CShader();
};

