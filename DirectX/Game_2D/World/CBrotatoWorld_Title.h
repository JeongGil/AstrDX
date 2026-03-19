#pragma once
#include <World/CWorld.h>

class CBrotatoWorld_Title :
    public CWorld
{
public:
	bool Init() override;

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
};

