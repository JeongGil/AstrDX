#pragma once

#include "Sync.h"

struct FThreadQueueData
{
	int Header = 0;
	int Size = 0;
	unsigned char Data[1024] = {};
};

class CThreadBase;

class CThreadQueue
{
	friend CThreadBase;

	inline const static int MAX_DATA = 200;

public:
	CThreadQueue();
	~CThreadQueue();

private:
	FThreadQueueData Data[MAX_DATA];
	int Push = 0;
	int Pop = 0;
	int Size = 0;

	CRITICAL_SECTION Crt;

public:
	void push(int Header, int Size, unsigned char* Data);
	void pop(int& Header, int& Size, unsigned char* Data);
	int size();
	bool full();
	bool empty();
};

