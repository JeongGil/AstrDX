#pragma once

#include "../EngineInfo.h"
#include "CWorld.h"

class CWorldManager
{
public:
	template <typename T>
	std::weak_ptr<T> CreateWorld(bool bNext = true)
	{
		std::shared_ptr<CWorld>& TargetWorld = bNext ? NextWorld : World;

		TargetWorld = std::make_shared<T>();
		if (!TargetWorld || !TargetWorld->Init())
		{
			return std::weak_ptr<T>();
		}

		return std::dynamic_pointer_cast<T>(TargetWorld);
	}

public:
	std::weak_ptr<CWorld> GetWorld() const
	{
		return World;
	}

private:
	std::shared_ptr<CWorld> World;
	std::shared_ptr<CWorld> NextWorld;

public:
	bool Init();
	void Update(const float DeltaTime);
	void PostUpdate(const float DeltaTime);
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

