#include "CThreadBase.h"

CThreadBase::CThreadBase()
{
}

CThreadBase::~CThreadBase()
{
}

void CThreadBase::Init(bool Pause)
{
}

void CThreadBase::Exit()
{
}

void CThreadBase::AddData(int Header, int Size, unsigned char* Data)
{
}

void CThreadBase::GetData(int& Header, int& Size, unsigned char* Data)
{
}

int CThreadBase::GetQueueSize()
{
}

bool CThreadBase::EmptyQueue()
{
}

void CThreadBase::Suspend()
{
}

void CThreadBase::Resume()
{
}

unsigned int CThreadBase::ThreadFunc(void* Arg)
{
}
