#pragma once

#include "EngineInfo.h"

class CSync
{
public:
	CSync(CRITICAL_SECTION* Crt)
	{
		this->Crt = Crt;

		EnterCriticalSection(Crt);
	}

	~CSync()
	{
		if (Crt)
		{
			LeaveCriticalSection(Crt);
		}
	}

private:
	CRITICAL_SECTION* Crt = nullptr;
};
