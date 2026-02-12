#pragma once
#include <World/CWorld.h>

#include "../ClientInfo.h"

class CLoadingThread;

class CLoadingWorld :
    public CWorld
{
public:
	CLoadingWorld();
	~CLoadingWorld() override;

private:
	std::shared_ptr<CLoadingThread> Thread;
	EWorldType LoadType;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Load(EWorldType WorldType);

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
};

