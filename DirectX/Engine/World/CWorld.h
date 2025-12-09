#pragma once

#include "../EngineInfo.h"

class CWorld
{	
public:
	bool Init();
	void Update(float deltaTime);
	void Render();

public:
	CWorld();
	~CWorld();
};

