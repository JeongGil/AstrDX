#include "CThreadManager.h"

CThreadManager::CThreadManager()
{
}

CThreadManager::~CThreadManager()
{
	for (auto& Thread : Threads | std::views::values)
	{
		Thread->Exit();
	}
}

bool CThreadManager::Init()
{
	return true;
}

bool CThreadManager::DeleteThread(const std::string& Key)
{
	auto It = Threads.find(Key);

	if (It == Threads.end())
	{
		return false;
	}

	It->second->Exit();

	Threads.erase(It);

	return true;
}
