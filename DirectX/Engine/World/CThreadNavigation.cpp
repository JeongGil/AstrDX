#include "CThreadNavigation.h"

#include <thread>

#include "CNavAgent.h"
#include "CNavigation.h"
#include "CWorldNavigation.h"
#include "../Component/CComponent.h"


CThreadNavigation::CThreadNavigation()
{
}

CThreadNavigation::~CThreadNavigation()
{
}

void CThreadNavigation::Exit()
{
	CThreadBase::Exit();
}

void CThreadNavigation::Run()
{
	bLoop = true;

	while (bLoop)
	{
		if (!Queue->empty())
		{
			int	Header = 0;
			int	Size = 0;
			UCHAR Data[1024] = {};

			Queue->pop(Header, Size, Data);

			switch (Header)
			{
				case ENavigationHeader::FindPath:
				{
					FVector Start, End;

					int DataSize = 0;

					memcpy(&Start, Data, sizeof(FVector));
					DataSize += sizeof(FVector);
					memcpy(&End, Data + DataSize, sizeof(FVector));
					DataSize += sizeof(FVector);

					CComponent* AgPtr = nullptr;
					memcpy(&AgPtr, Data + DataSize, sizeof(CComponent*));

					std::shared_ptr<CNavAgent> Agent = std::dynamic_pointer_cast<CNavAgent>(AgPtr->shared_from_this());

					// Performs pathfinding.
					std::list<FVector>	PathList;

					if (Navigation->FindPath(Start, End, PathList))
					{
						// If a path is found, it adds the route.
						memset(Data, 0, 1024);
						DataSize = 0;

						memcpy(Data, &AgPtr, sizeof(CComponent*));
						DataSize += sizeof(CComponent*);

						int	Count = (int)PathList.size();
						memcpy(Data + DataSize, &Count, sizeof(int));
						DataSize += sizeof(int);

						for (const auto& Path : PathList)
						{
							FVector Pos = Path;
							memcpy(Data + DataSize, &Pos, sizeof(FVector3));
							DataSize += sizeof(FVector3);
						}

						if (auto WorldNav = WorldNavigation.lock())
						{
							WorldNav->AddData(ENavigationHeader::FindComplete, DataSize, Data);
						}
					}

					break;
				}
				case ENavigationHeader::Exit:
				{
					bLoop = false;
					return;
				}
				default:
					break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void CThreadNavigation::SetWorldNavigation(const std::weak_ptr<CWorldNavigation>& Nav)
{
	WorldNavigation = Nav;
}

void CThreadNavigation::SetTileMap(const std::weak_ptr<CTileMapComponent>& TileMap)
{
	Navigation.reset(new CNavigation);
	Navigation->SetTileMap(TileMap);
}
