#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderTexture2DInstancing :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderTexture2DInstancing();

public:
	~CShaderTexture2DInstancing() override;

	bool Init(const std::string& PathName) override;
};

