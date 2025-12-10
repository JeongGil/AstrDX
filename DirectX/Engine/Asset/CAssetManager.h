#pragma once

#include "../EngineInfo.h"

class CMeshManager;

class CAssetManager
{
public:
	bool Init();

	CMeshManager* GetMeshManager() const
	{
		return meshManager.get();
	}

private:
	std::shared_ptr<CMeshManager> meshManager;

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

