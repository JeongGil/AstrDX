#pragma once
#include <World/CWorld.h>

class CStartWorld :
    public CWorld
{
public:
	CStartWorld();
	~CStartWorld() override;

	bool Init() override;

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
};

