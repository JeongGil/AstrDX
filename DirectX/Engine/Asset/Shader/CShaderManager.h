#pragma once

#include "../../EngineInfo.h"
#include "CShader.h"

class CShaderManager
{
	friend class CAssetManager;

public:
	bool Init();

	std::weak_ptr<CShader> FindShader(const std::string& Name);

	template <typename T>
	bool CreateShader(const std::string& Name)
	{
		if (Shaders.contains(Name))
		{
			return false;
		}

		auto ShaderInstance = new T;
		std::shared_ptr<CShader> NewShader;
		NewShader.reset(ShaderInstance);
		if (!NewShader->Init())
		{
			return false;
		}

		Shaders.emplace(Name, NewShader);

		return true;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<CShader>> Shaders;

private:
	CShaderManager();

public:
	~CShaderManager();
};

