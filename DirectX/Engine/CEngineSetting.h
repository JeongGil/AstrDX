#pragma once

#include "EngineInfo.h"

class CEngineSetting
{
	friend class CEngine;

public:
	virtual bool Init();

protected:
	CEngineSetting() = default;

public:
	~CEngineSetting() = default;
};

