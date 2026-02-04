#pragma once
#include <Asset/Shader/CGraphicShader.h>

class CShaderManager;

class CShaderPostProcessHit :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderPostProcessHit();

public:
	~CShaderPostProcessHit() override;

	bool Init(const std::string& PathName) override;
};

