#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderBlur :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderBlur();

public:
	~CShaderBlur() override;

	bool Init(const std::string& PathName) override;
};

