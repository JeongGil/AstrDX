#include "CShaderManager.h"

#include "CConstantBuffer.h"
#include "CShaderColor2D.h"

bool CShaderManager::Init()
{
	// ConstantBuffer
	if (!CreateCBuffer("Transform", sizeof(FCBufferTransformData), 0, EShaderBufferType::GRAPHIC))
	{
		return false;
	}

	// Shader
	if (!CreateShader<CShaderColor2D>("Color2D"))
	{
		return false;
	}

	return true;
}

std::weak_ptr<CShader> CShaderManager::FindShader(const std::string& Key)
{
	const auto it = Shaders.find(Key);
	return it == Shaders.end() ? std::weak_ptr<CShader>() : it->second;
}

std::weak_ptr<CConstantBuffer> CShaderManager::FindCBuffer(const std::string& Key)
{
	const auto it = CBuffers.find(Key);
	return it == CBuffers.end() ? std::weak_ptr<CConstantBuffer>() : it->second;
}

bool CShaderManager::CreateCBuffer(const std::string& Key, int Size, int Register, int ShaderBuffer)
{
	if (CBuffers.contains(Key))
	{
		return false;
	}

	auto NewBuffer = new CConstantBuffer;
	std::shared_ptr<CConstantBuffer> Shared;
	Shared.reset(NewBuffer);

	if (!Shared->Init(Size, Register, ShaderBuffer))
	{
		return false;
	}

	CBuffers.emplace(Key, Shared);

	return true;
}
