#pragma once
#include "World/CWorld.h"

class CMainWorld :
    public CWorld
{
public:
	CMainWorld() = default;
	~CMainWorld() override = default;

	bool Init() override;

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
};

