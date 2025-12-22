#pragma once
#include "CShader.h"
class CComputeShader :
    public CShader
{
	friend class CShaderManager;

public:
	bool Init(const std::string& PathName) override = 0;
	void SetShader() final;

protected:
	CComputeShader();

public:
	~CComputeShader() override = 0;
};

