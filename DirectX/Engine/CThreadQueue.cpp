#include "CThreadQueue.h"

CThreadQueue::CThreadQueue()
{
	InitializeCriticalSection(&Crt);
}

CThreadQueue::~CThreadQueue()
{
	DeleteCriticalSection(&Crt);
}

void CThreadQueue::push(int Header, int Size, unsigned char* Data)
{
	CSync _(&Crt);

	if (this->Size == MAX_DATA)
	{
		return;
	}

	Push = ++Push % 200;

	this->Data[Push].Header = Header;
	this->Data[Push].Size = Size;

	if (Data)
	{
		memcpy(this->Data[Push].Data, Data, Size);
	}

	++this->Size;
}

void CThreadQueue::pop(int& Header, int& Size, unsigned char* Data)
{
	CSync _(&Crt);

	if (this->Size == 0)
	{
		return;
	}

	Pop = ++Pop % 200;

	Header = this->Data[Pop].Header;
	Size = this->Data[Pop].Size;

	if (Size > 0)
	{
		memcpy(Data, this->Data[Pop].Data, Size);
	}

	--this->Size;
}

int CThreadQueue::size()
{
	CSync _(&Crt);

	return Size;
}

bool CThreadQueue::full()
{
	CSync _(&Crt);

	return Size == MAX_DATA;
}

bool CThreadQueue::empty()
{
	CSync _(&Crt);

	return Size == 0;
}
