#pragma once
#include "CGraphicShader.h"

class CShaderTexture2D :
    public CGraphicShader
{
	friend class CShaderManager;

protected:
	CShaderTexture2D() = default;

public:
	~CShaderTexture2D() override = default;

	bool Init(const std::string& PathName) override;
};

