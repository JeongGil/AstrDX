#include "CLoadingThread.h"

#include <World/CWorldManager.h>

#include "../World/CMainWorld.h"
#include "../World/CStartWorld.h"

CLoadingThread::CLoadingThread()
{
}

CLoadingThread::~CLoadingThread()
{
}

void CLoadingThread::Exit()
{
	CThreadBase::Exit();
}

void CLoadingThread::Run()
{
	switch (WorldType)
	{
	case EWorldType::Start:
		CWorldManager::GetInst()->CreateAsyncWorld<CStartWorld>();
		break;
	case EWorldType::Main:
		CWorldManager::GetInst()->CreateAsyncWorld<CMainWorld>();
		break;
	}
}
