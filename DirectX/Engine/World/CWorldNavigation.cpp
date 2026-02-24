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
		Thread->AddData(ENavigationHeader::Exit, 0, nullptr);

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

			Thread->SetWorldNavigation(std::dynamic_pointer_cast<CWorldNavigation>(shared_from_this()));
			Thread->SetTileMap(TileMap);

			Threads.push_back(Thread);
		}
	}
}

void CWorldNavigation::FindPath(const FVector2& Start, const FVector2& End)
{
	int	QueueCount = INT_MAX;
	int	Index = -1;

	size_t	Size = Threads.size();

	for (size_t i = 0; i < Size; ++i)
	{
		int	Count = Threads[i]->GetQueueSize();

		if (Count < QueueCount)
		{
			Index = (int)i;

			if (Count == 0)
			{
				break;
			}

			QueueCount = Count;
		}
	}

	int	Header = ENavigationHeader::FindPath;

	UCHAR Data[1024] = {};

	int	DataSize = 0;

	memcpy(Data, &Start, sizeof(FVector2));
	DataSize += sizeof(FVector2);

	memcpy(Data + DataSize, &End, sizeof(FVector2));
	DataSize += sizeof(FVector2);

	Threads[Index]->AddData(Header, DataSize, Data);
}

void CWorldNavigation::Begin()
{
	auto ThreadView = Threads | std::views::filter([](const auto& Thread) {return Thread != nullptr; });
	for (const auto& Thread : ThreadView)
	{
		Thread->Resume();
	}
}
