#pragma once

#include "CGraphicShader.h"

class CShaderColor2D
	: public CGraphicShader
{
	friend class CShaderManager;

public:
	bool Init(const std::string& PathName) override;

protected:
	CShaderColor2D();

public:
	~CShaderColor2D() override;
};

