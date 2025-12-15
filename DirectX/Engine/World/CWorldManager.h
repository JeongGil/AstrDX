#pragma once

#include "../EngineInfo.h"

class CWorld;
class CWorldManager
{
public:
	std::weak_ptr<CWorld> GetWorld() const
	{
		return World;
	}

private:
	std::shared_ptr<CWorld> World;

public:
	bool Init();
	void Update(float DeltaTime);
	void Render();

public:
	static CWorldManager* GetInst()
	{
		if (Instance == nullptr)
		{
			Instance = new CWorldManager;
		}

		return Instance;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(Instance)
	}

private:
	CWorldManager();
	~CWorldManager();

	inline static CWorldManager* Instance = nullptr;
};

