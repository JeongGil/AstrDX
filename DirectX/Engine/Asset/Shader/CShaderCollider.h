#pragma once
#include "CGraphicShader.h"

class CShaderCollider :
    public CGraphicShader
{
	friend class CShaderManager;

public:
	bool Init(const std::string& PathName) override;

protected:
	CShaderCollider() = default;

public:
	~CShaderCollider() override = default;
};

