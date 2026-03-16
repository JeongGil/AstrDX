#pragma once

#include <Asset/Shader/CGraphicShader.h>

class CShaderBrotatoTile :
	public CGraphicShader
{
public:
	CShaderBrotatoTile();
	~CShaderBrotatoTile() override;

public:
	bool Init(const std::string& PathName) override;
};
