#pragma once
#include "CEngineSetting.h"

class CGlobalSetting :
    public CEngineSetting
{
	friend class CEngine;

public:
	bool Init() override;

private:
	CGlobalSetting() = default;

public:
	~CGlobalSetting() = default;
};

