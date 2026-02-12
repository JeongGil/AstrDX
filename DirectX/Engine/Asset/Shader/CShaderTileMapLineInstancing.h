#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderTileMapLineInstancing :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderTileMapLineInstancing();

public:
	~CShaderTileMapLineInstancing() override;

	bool Init(const std::string& PathName) override;
};

