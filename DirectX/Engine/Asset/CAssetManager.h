#pragma once

#include "../EngineInfo.h"

class CMeshManager;
class CShaderManager;

class CAssetManager
{
public:
	bool Init();

	[[nodiscard]] std::weak_ptr<CMeshManager> GetMeshManager() const
	{
		return MeshManager;
	}

	[[nodiscard]] std::weak_ptr<CShaderManager> GetShaderManager() const
	{
		return ShaderManager;
	}

private:
	std::shared_ptr<CMeshManager> MeshManager;
	std::shared_ptr<CShaderManager> ShaderManager;

public:
	static CAssetManager* GetInst()
	{
		if (inst == nullptr)
		{
			inst = new CAssetManager;
		}

		return inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(inst)
	}

private:
	CAssetManager();
	~CAssetManager();

	inline static CAssetManager* inst = nullptr;
};

