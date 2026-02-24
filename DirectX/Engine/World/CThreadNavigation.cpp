#include "CThreadNavigation.h"

#include <thread>

#include "CNavigation.h"

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
					FVector2 Start, End;

					memcpy(&Start, Data, sizeof(FVector2));
					memcpy(&End, Data + sizeof(FVector2),
						sizeof(FVector2));

					// Performs pathfinding.
					std::list<FVector2>	PathList;

					if (Navigation->FindPath(Start, End, PathList))
					{
						// If a path is found, it adds the route.
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
