#pragma once

#include "../EngineInfo.h"

class CWorld;
class CWorldManager
{
private:
	std::shared_ptr<CWorld> world;

public:
	bool Init();
	void Update(float deltaTime);
	void Render();

public:
	static CWorldManager* GetInst()
	{
		if (inst == nullptr)
		{
			inst = new CWorldManager;
		}

		return inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(inst)
	}

private:
	CWorldManager();
	~CWorldManager();

	inline static CWorldManager* inst = nullptr;
};

