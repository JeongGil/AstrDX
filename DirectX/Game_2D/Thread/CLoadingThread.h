#pragma once
#include <CThreadBase.h>

#include "../ClientInfo.h"

class CThreadManager;

class CLoadingThread :
    public CThreadBase
{
	friend CThreadManager;

protected:
	CLoadingThread();

public:
	~CLoadingThread() override;

private:
	EWorldType WorldType = EWorldType::None;

public:
	void Exit() override;
	void Run() override;

	void SetWorldType(EWorldType Type)
	{
		WorldType = Type;
	}
};

