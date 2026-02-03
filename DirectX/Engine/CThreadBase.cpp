#include "CThreadBase.h"

CThreadBase::CThreadBase()
{
}

CThreadBase::~CThreadBase()
{
	bLoop = false;

	if (Thread)
	{
		WaitForSingleObject(Thread, INFINITE);

		CloseHandle(Thread);
	}

	DeleteCriticalSection(&Crt);
}

bool CThreadBase::Init(bool Pause)
{
	Queue.reset(new CThreadQueue);

	InitializeCriticalSection(&Crt);

	uint32_t InitFlag = Pause? CREATE_SUSPENDED : 0;

	Thread = (HANDLE)_beginthreadex(nullptr, 0, CThreadBase::ThreadFunc, (void*)this, InitFlag, nullptr);

	if (!Thread)
	{
		return false;
	}

	return true;
}

void CThreadBase::Exit()
{
	bLoop = false;

	TerminateThread(Thread, 0);
}

void CThreadBase::AddData(int Header, int Size, unsigned char* Data)
{
	Queue->push(Header, Size, Data);
}

void CThreadBase::GetData(int& Header, int& Size, unsigned char* Data)
{
	Queue->pop(Header, Size, Data);
}

int CThreadBase::GetQueueSize()
{
	return Queue->size();
}

bool CThreadBase::EmptyQueue()
{
	return Queue->empty();
}

void CThreadBase::Suspend()
{
	if (Thread)
	{
		SuspendThread(Thread);
	}
}

void CThreadBase::Resume()
{
	if (Thread)
	{
		ResumeThread(Thread);
	}
}

unsigned int CThreadBase::ThreadFunc(void* Arg)
{
	auto Thread = static_cast<CThreadBase*>(Arg);

	Thread->Run();

	{
		CSync _(&Thread->Crt);

		Thread->bComplete = true;
	}

	return 0;
}
