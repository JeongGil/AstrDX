#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderUIDefault :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderUIDefault();

public:
	~CShaderUIDefault() override;

public:
	bool Init(const std::string& PathName) override;
};

