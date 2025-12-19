#pragma once

#include <EngineInfo.h>

class CStateInterface abstract
{
public:
	void AddHP(int Added)
	{
		StateHP += Added;

#ifdef _DEBUG
		char	Test[256] = {};
		sprintf_s(Test, "HP : %d\n", StateHP);
		OutputDebugStringA(Test);
#endif
	}

protected:
	int StateHP = 10;

public:
	CStateInterface() = default;
	virtual ~CStateInterface() = default;
};

