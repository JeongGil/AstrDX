#pragma once

#include "../EngineInfo.h"

class CWorld;

class CWorldUIManager
{
	friend CWorld;

private:
	CWorldUIManager();

public:
	~CWorldUIManager();

private:
	std::weak_ptr<CWorld> World;

public:
	bool Init();
	void Update(const float DeltaTime);
};

