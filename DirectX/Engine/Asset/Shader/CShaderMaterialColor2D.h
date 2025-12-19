#pragma once
#include "CGraphicShader.h"

class CShaderMaterialColor2D :
    public CGraphicShader
{
	friend class CShaderManager;

public:
	bool Init() override;

protected:
	CShaderMaterialColor2D() = default;

public:
	~CShaderMaterialColor2D() override = default;
};

