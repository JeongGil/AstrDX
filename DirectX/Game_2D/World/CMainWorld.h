#pragma once
#include "World/CWorld.h"

class CMainWorld :
    public CWorld
{
public:
	bool Init() override;

public:
	CMainWorld() = default;
	~CMainWorld() override = default;
};

