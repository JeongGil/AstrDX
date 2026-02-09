#pragma once
#include "CGraphicShader.h"

class CShaderManager;

class CShaderTileMap :
    public CGraphicShader
{
	friend CShaderManager;

protected:
	CShaderTileMap();

public:
	~CShaderTileMap() override;

	bool Init(const std::string& PathName) override;
};

