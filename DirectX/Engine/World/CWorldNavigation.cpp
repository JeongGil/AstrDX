#include "CWorldNavigation.h"

#include "CThreadNavigation.h"
#include "../CThreadManager.h"

CWorldNavigation::CWorldNavigation()
{
}

CWorldNavigation::~CWorldNavigation()
{
	for (const auto& Thread : Threads)
	{
		const auto& Key = Thread->GetKey();

		CThreadManager::GetInst().DeleteThread(Key);
	}
}

bool CWorldNavigation::Init()
{
	return true;
}

void CWorldNavigation::Update(const float DeltaTime)
{
}

void CWorldNavigation::CreateNavigationThread(int Count, const std::weak_ptr<CTileMapComponent>& TileMap)
{
	if (auto World = this->World.lock())
	{
		UINT64 Addr = (UINT64)World.get();

		char WorldAddr[64] = {};
		sprintf_s(WorldAddr, "%llu_Thread", Addr);

		for (int i = 0; i < Count; i++)
		{
			char ThreadName[128] = {};
			sprintf_s(ThreadName, "%s%d", WorldAddr, i);

			auto Thread = CThreadManager::GetInst().Create<CThreadNavigation>(ThreadName, true);

			Thread->SetTileMap(TileMap);

			Threads.push_back(Thread);
		}
	}
}

void CWorldNavigation::Begin()
{
	for (const auto& Thread : Threads)
	{
		Thread->Resume();
	}
}
