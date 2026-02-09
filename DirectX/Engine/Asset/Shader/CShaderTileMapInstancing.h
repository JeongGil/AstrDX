#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderTileMapInstancing :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderTileMapInstancing();

public:
	~CShaderTileMapInstancing() override;

	bool Init(const std::string& PathName) override;	
};

