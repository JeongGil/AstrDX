#include "CShaderManager.h"

#include "CConstantBuffer.h"
#include "CShaderCollider.h"
#include "CShaderColor2D.h"
#include "CShaderMaterialColor2D.h"
#include "CShaderTexture2D.h"
#include "CShaderUIDefault.h"

bool CShaderManager::Init()
{
	// ConstantBuffer
	if (!CreateCBuffer("Transform", sizeof(FCBufferTransformData), 0, EShaderBufferType::GRAPHIC))
	{
		return false;
	}

	if (!CreateCBuffer("Material", sizeof(FCBufferMaterialData), 1, EShaderBufferType::Pixel))
	{
		return false;
	}

	if (!CreateCBuffer("Animation2D", sizeof(FCBufferAnimation2DData), 2, EShaderBufferType::Vertex))
	{
		return false;
	}

	if (!CreateCBuffer("Collider", sizeof(FCBufferColliderData), 10, EShaderBufferType::Pixel))
	{
		return false;
	}

	if (!CreateCBuffer("UIDefault", sizeof(FCBufferUIDefaultData), 10, EShaderBufferType::VP))
	{
		return false;
	}

	// Shader
	if (!CreateShader<CShaderColor2D>("Color2D", "EngineShader"))
	{
		return false;
	}

	if (!CreateShader<CShaderMaterialColor2D>("MaterialColor2D", "EngineShader"))
	{
		return false;
	}

	if (!CreateShader<CShaderTexture2D>("DefaultTexture2D", "EngineShader"))
	{
		return false;
	}

	if (!CreateShader<CShaderCollider>("Collider", "EngineShader"))
	{
		return false;
	}

	if (!CreateShader<CShaderUIDefault>("UIDefault", "EngineShader"))
	{
		return false;
	}

	return true;
}

void CShaderManager::ReleaseShader(const std::string& Key)
{
	auto It = Shaders.find(Key);
	if (It != Shaders.end())
	{
		if (It->second.use_count() == 1)
		{
			Shaders.erase(It);
		}
	}
}

void CShaderManager::ReleaseCBuffer(const std::string& Key)
{
	auto It = CBuffers.find(Key);
	if (It != CBuffers.end())
	{
		if (It->second.use_count() == 1)
		{
			CBuffers.erase(It);
		}
	}
}

std::weak_ptr<CShader> CShaderManager::FindShader(const std::string& Key)
{
	const auto it = Shaders.find(Key);
	if (it == Shaders.end())
	{
		return std::weak_ptr<CShader>();
	}
	else
	{
		return it->second;
	}
}

std::weak_ptr<CConstantBuffer> CShaderManager::FindCBuffer(const std::string& Key)
{
	const auto it = CBuffers.find(Key);
	if (it == CBuffers.end())
	{
		return std::weak_ptr<CConstantBuffer>();
	}
	else
	{
		return it->second;
	}
}

bool CShaderManager::CreateCBuffer(const std::string& Key, int Size, int Register, int ShaderBuffer)
{
	if (CBuffers.contains(Key))
	{
		return true;
	}

	std::shared_ptr<CConstantBuffer> Shared(new CConstantBuffer);
	if (!Shared->Init(Size, Register, ShaderBuffer))
	{
		return false;
	}

	Shared->SetKey(Key);

	CBuffers.emplace(Key, Shared);

	return true;
}
