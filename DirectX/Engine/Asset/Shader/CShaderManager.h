#pragma once

#include "CBufferContainer.h"
#include "../../EngineInfo.h"
#include "CShader.h"

class CConstantBuffer;

class CShaderManager
{
	friend class CAssetManager;

public:
	bool Init();

	std::weak_ptr<CShader> FindShader(const std::string& Key);
	std::weak_ptr<CConstantBuffer> FindCBuffer(const std::string& Key);

	template <typename T>
	bool CreateShader(const std::string& Key)
	{
		if (Shaders.contains(Key))
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

		Shaders.emplace(Key, NewShader);

		return true;
	}

	bool CreateCBuffer(const std::string& Key, int Size, int Register, int ShaderBuffer = EShaderBufferType::VP);

private:
	std::unordered_map<std::string, std::shared_ptr<CShader>> Shaders;
	std::unordered_map<std::string, std::shared_ptr<CConstantBuffer>> CBuffers;

private:
	CShaderManager() = default;

public:
	~CShaderManager() = default;
};

