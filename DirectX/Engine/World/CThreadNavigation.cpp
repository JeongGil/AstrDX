#include "CThreadNavigation.h"

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
		
	}
}

void CThreadNavigation::SetTileMap(const std::weak_ptr<CTileMapComponent>& TileMap)
{
	this->TileMap = TileMap;
}
