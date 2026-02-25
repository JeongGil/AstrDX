#include "CWorldNavigation.h"

#include "../World/CNavAgent.h"
#include "CThreadNavigation.h"
#include "../CThreadManager.h"
#include "../Component/CComponent.h"

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
	NavQueue.reset(new CThreadQueue);

	return true;
}

void CWorldNavigation::Update(const float DeltaTime)
{
	int QueueSize = NavQueue->size();

	int Header = 0;
	int Size = 0;
	UCHAR Data[1024] = {};

	for (int i = 0; i < QueueSize; i++)
	{
		NavQueue->pop(Header, Size, Data);

		switch (Header)
		{
			case ENavigationHeader::FindComplete:
				NavigationComplete(Size, Data);
				break;
			default:
				break;
		}
	}
}

void CWorldNavigation::AddData(int Header, int Size, UCHAR* Data)
{
	NavQueue->push(Header, Size, Data);
}

void CWorldNavigation::NavigationComplete(int Size, UCHAR* Data)
{
	int	DataSize = 0;

	CComponent* AgPtr = nullptr;

	memcpy(&AgPtr, Data, sizeof(CComponent*));
	DataSize += sizeof(CComponent*);

	std::shared_ptr<CNavAgent> Agent = std::dynamic_pointer_cast<CNavAgent>(AgPtr->shared_from_this());

	int	PathCount = 0;
	memcpy(&PathCount, Data + DataSize, sizeof(int));
	DataSize += sizeof(int);

	Agent->StartPathPoint();

	for (int i = 0; i < PathCount; ++i)
	{
		FVector Pos;
		memcpy(&Pos, Data + DataSize, sizeof(FVector3));
		DataSize += sizeof(FVector3);

		Agent->AddPathPoint(Pos);
	}

	Agent->StartPath();
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

void CWorldNavigation::FindPath(const FVector& Start, const FVector& End, CComponent* Agent)
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

	memcpy(Data, &Start, sizeof(FVector));
	DataSize += sizeof(FVector);

	memcpy(Data + DataSize, &End, sizeof(FVector));
	DataSize += sizeof(FVector);

	memcpy(Data + DataSize, &Agent, sizeof(CComponent*));
	DataSize += sizeof(CComponent*);

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
