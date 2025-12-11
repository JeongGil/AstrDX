#include "CShaderManager.h"

#include "CShaderColor2D.h"

bool CShaderManager::Init()
{
	if (!CreateShader<CShaderColor2D>("Color2D"))
	{
		return false;
	}

	return true;
}

std::weak_ptr<CShader> CShaderManager::FindShader(const std::string& Name)
{
	const auto it = Shaders.find(Name);
	return it == Shaders.end() ? std::shared_ptr<CShader>() : it->second;
}

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}
