#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderNullBuffer :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderNullBuffer();

public:
	~CShaderNullBuffer() override;

	bool Init(const std::string& PathName) override;
};

