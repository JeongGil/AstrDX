#pragma once

#include "CThreadQueue.h"
#include "EngineInfo.h"
#include "Sync.h"

class CThreadManager;

class CThreadBase abstract
{
	friend CThreadManager;

protected:
	CThreadBase();

public:
	virtual ~CThreadBase();

protected:
	std::string Key;
	HANDLE Thread = nullptr;
	std::shared_ptr<CThreadQueue> Queue;
	bool bLoop = false;
	bool bComplete = false;

	CRITICAL_SECTION Crt;

public:
	bool Init(bool Pause = false);
	virtual void Exit();
	virtual void Run() = 0;

	void AddData(int Header, int Size, unsigned char* Data);
	void GetData(int& Header, int& Size, unsigned char* Data);
	int GetQueueSize();
	bool EmptyQueue();

	void Suspend();
	void Resume();

private:
	static unsigned int __stdcall ThreadFunc(void* Arg);

public:
	void SetKey(const std::string& Key)
	{
		this->Key = Key;
	}

	[[nodiscard]] bool IsComplete()
	{
		CSync _(&Crt);

		return bComplete;
	}
};

