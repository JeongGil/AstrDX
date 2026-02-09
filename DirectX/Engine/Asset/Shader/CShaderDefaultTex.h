#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderDefaultTex :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderDefaultTex();

public:
	~CShaderDefaultTex() override;

	bool Init(const std::string& PathName) override;
};

