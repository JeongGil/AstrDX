#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderMaterialColor2DInstancing :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderMaterialColor2DInstancing();

public:
	~CShaderMaterialColor2DInstancing() override;

	bool Init(const std::string& PathName) override;
};

