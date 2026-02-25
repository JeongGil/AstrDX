#include "CNavigation.h"

#include "../Component/CTile.h"
#include "../Component/CTileMapComponent.h"

CNavigation::CNavigation()
{
}

CNavigation::~CNavigation()
{
}

void CNavigation::SetTileMap(const std::weak_ptr<CTileMapComponent>& TileMap)
{
	this->TileMap = TileMap;

	if (auto Map = TileMap.lock())
	{
		Shape = Map->GetTileShape();
		CountX = Map->GetTileCountX();
		CountY = Map->GetTileCountY();
		TileSize = Map->GetTileSize();
		DiagonalCost = TileSize.Length();

		int Count = CountX * CountY;

		Nodes.resize(Count);
		OpenNodes.reserve(Count);
		UsedNodes.reserve(Count);

		for (int i = 0; i < Count; i++)
		{
			if (auto Tile = Map->GetTile(i).lock())
			{
				auto& Node = Nodes[i];

				Node.Pos = Tile->GetPos();
				Node.Size = TileSize;
				Node.Center = Tile->GetCenter();
				Node.Index = i;

				const auto& [Quot, Rem] = std::div(i, CountX);
				Node.IndexX = Rem;
				Node.IndexY = Quot;
			}
		}
	}
}

bool CNavigation::FindPath(const FVector& Start, const FVector& End, std::list<FVector>& Paths)
{
	auto TileMap = this->TileMap.lock();

	if (!TileMap)
	{
		Paths.clear();
		Paths.emplace_back(End);

		return false;
	}

	// Retrieve indices for start and end positions.
	int	StartIndex = TileMap->GetTileIndex(Start);
	if (StartIndex == -1)
	{
		return false;
	}

	int	EndIndex = TileMap->GetTileIndex(End);
	if (EndIndex == -1)
	{
		return false;
	}

	// If the destination is an unreachable tile (UnableToMove):
	// 1. Stop the search.
	// 2. Search the 8 surrounding directions to find the tile closest to the start point
	//    and use it as the new destination.
	if (TileMap->GetTileType(EndIndex) == ETileType::UnableToMove)
	{
		// Case: Stop the search
		//PathList.clear();
		//return false;

		// Case: Find a nearby alternative tile
		const auto& [StartQuot, StartRem] = std::div(StartIndex, CountX);
		int	StartX = StartRem;
		int	StartY = StartQuot;

		const auto& [EndQuot, EndRem] = std::div(EndIndex, CountX);
		int	EndX = EndRem;
		int	EndY = EndQuot;

		int	FindIndex[8] =
		{
			(EndY + 1) * CountX + EndX,
			(EndY + 1) * CountX + (EndX + 1),
			EndY * CountX + (EndX + 1),
			(EndY - 1) * CountX + (EndX + 1),
			(EndY - 1) * CountX + EndX,
			(EndY - 1) * CountX + (EndX - 1),
			EndY * CountX + (EndX - 1),
			(EndY + 1) * CountX + (EndX - 1)
		};

		std::vector<int> CheckIndexes;

		for (const auto& CheckIndex : CheckIndexes)
		{
			if (TileMap->GetTileType(CheckIndex) == ETileType::Normal)
			{
				CheckIndexes.push_back(CheckIndex);
			}
		}

		float Dist = FLT_MAX;
		for (const auto& CheckIndex : CheckIndexes)
		{
			float CheckDist = Nodes[CheckIndex].Center.Distance(Nodes[StartIndex].Center);

			if (CheckDist < Dist)
			{
				EndIndex = CheckIndex;
				Dist = CheckDist;
			}
		}
	}

	Paths.clear();

	// Reset all data for nodes that were used in previous searches.
	for (const auto& UsedNode : UsedNodes)
	{
		UsedNode->NodeType = ENavNodeType::None;
		UsedNode->Dist = FLT_MAX;
		UsedNode->Heuristic = FLT_MAX;
		UsedNode->Total = FLT_MAX;
		UsedNode->Parent = nullptr;
		UsedNode->SearchDirs.clear();
	}

	UsedNodes.clear();

	// Get the start and end nodes.
	FNavNode* StartNode = &Nodes[StartIndex];
	FNavNode* EndNode = &Nodes[EndIndex];

	if (StartNode == EndNode)
	{
		Paths.emplace_back(End);
		return true;
	}

	StartNode->NodeType = ENavNodeType::Open;
	StartNode->Dist = 0.f;
	StartNode->Heuristic = End.Distance(StartNode->Center);
	StartNode->Total = StartNode->Heuristic;

	// The initial starting node begins by checking all 8 directions.
	for (int i = 0; i < ESearchDir::End; ++i)
	{
		StartNode->SearchDirs.emplace_back((ESearchDir::Type)i);
	}

	OpenNodes.emplace_back(StartNode);
	UsedNodes.emplace_back(StartNode);

	while (!OpenNodes.empty())
	{
		// Retrieve the node to inspect.
		// The Open List is sorted in descending order so that the node 
		// with the lowest cost is at the back for efficient removal.
		FNavNode* Node = OpenNodes.back();

		// Remove from the Open List.
		OpenNodes.pop_back();

		// Mark this node as part of the Closed List.
		Node->NodeType = ENavNodeType::Close;

		// Use SearchDir to explore in specific directions and find nodes that form corners.
		// This function returns true if the destination is found.
		if (FindNode(Node, EndNode, End, Paths))
		{
			OpenNodes.clear();
			return true;
		}

		// Sort the Open List.
		if (OpenNodes.size() >= 2)
		{
			std::ranges::sort(OpenNodes, CNavigation::SortOpenNodes);
		}
	}

	OpenNodes.clear();

	return false;
}

bool CNavigation::FindNode(FNavNode* Node, FNavNode* EndNode, const FVector& End, std::list<FVector>& Paths)
{
	for (const auto& Dir : Node->SearchDirs)
	{
		// 현재 방향으로 탐색해나가며 코너를 찾아온다.
		FNavNode* Corner = GetCorner(Dir, Node, EndNode);

		// 코너가 없을 경우 다음 방향을 검사하게 한다.
		if (!Corner)
		{
			continue;
		}

		if (Corner == EndNode)
		{
			UsedNodes.emplace_back(Corner);

			// 도착점을 경로에 넣어준다.
			Paths.emplace_back(End);

			FNavNode* PathNode = Node;

			while (PathNode)
			{
				FVector Center;
				Center = PathNode->Center;

				Paths.emplace_front(Center);
				PathNode = PathNode->Parent;
			}

			// 시작노드의 위치는 필요없기 때문에 제거한다.
			Paths.pop_front();

			return true;
		}

		float Dist = Node->Dist + Node->Center.Distance(Corner->Center);

		// 구해준 코너가 열린목록에 이미 들어가 있는 노드일 경우
		if (Corner->NodeType == ENavNodeType::Open)
		{
			if (Corner->Dist > Dist)
			{
				Corner->Dist = Dist;
				Corner->Total = Dist + Corner->Heuristic;
				Corner->Parent = Node;

				AddDir(Dir, Corner);
			}
		}
		else
		{
			Corner->NodeType = ENavNodeType::Open;
			Corner->Dist = Dist;
			Corner->Heuristic = End.Distance(Corner->Center);
			Corner->Total = Dist + Corner->Heuristic;
			Corner->Parent = Node;

			OpenNodes.emplace_back(Corner);
			UsedNodes.emplace_back(Corner);

			AddDir(Dir, Corner);
		}
	}

	return false;
}

FNavNode* CNavigation::GetCorner(ESearchDir::Type Dir, FNavNode* Node, FNavNode* EndNode)
{
	switch (Shape)
	{
		case Rect:
			switch (Dir)
			{
				case ESearchDir::T:
					return GetCornerRectT(Node, EndNode);
				case ESearchDir::RT:
					return GetCornerRectRT(Node, EndNode);
				case ESearchDir::R:
					return GetCornerRectR(Node, EndNode);
				case ESearchDir::RB:
					return GetCornerRectRB(Node, EndNode);
				case ESearchDir::B:
					return GetCornerRectB(Node, EndNode);
				case ESearchDir::LB:
					return GetCornerRectLB(Node, EndNode);
				case ESearchDir::L:
					return GetCornerRectL(Node, EndNode);
				case ESearchDir::LT:
					return GetCornerRectLT(Node, EndNode);
				case ESearchDir::End:
					break;
			}
			break;
		case Isometric:
			switch (Dir)
			{
				case ESearchDir::T:
					break;
				case ESearchDir::RT:
					break;
				case ESearchDir::R:
					break;
				case ESearchDir::RB:
					break;
				case ESearchDir::B:
					break;
				case ESearchDir::LB:
					break;
				case ESearchDir::L:
					break;
				case ESearchDir::LT:
					break;
				case ESearchDir::End:
					break;
			}
			break;
	}

	return nullptr;
}

void CNavigation::AddDir(ESearchDir::Type Dir, FNavNode* Node)
{
	auto& SearchDirs = Node->SearchDirs;
	SearchDirs.clear();

	switch (Shape)
	{
		case Rect:
			switch (Dir)
			{
				case ESearchDir::T:
					SearchDirs.emplace_back(ESearchDir::T);
					SearchDirs.emplace_back(ESearchDir::RT);
					SearchDirs.emplace_back(ESearchDir::LT);
					break;
				case ESearchDir::RT:
					SearchDirs.emplace_back(ESearchDir::RT);
					SearchDirs.emplace_back(ESearchDir::T);
					SearchDirs.emplace_back(ESearchDir::R);
					SearchDirs.emplace_back(ESearchDir::LT);
					SearchDirs.emplace_back(ESearchDir::RB);
					break;
				case ESearchDir::R:
					SearchDirs.emplace_back(ESearchDir::R);
					SearchDirs.emplace_back(ESearchDir::RT);
					SearchDirs.emplace_back(ESearchDir::RB);
					break;
				case ESearchDir::RB:
					SearchDirs.emplace_back(ESearchDir::RB);
					SearchDirs.emplace_back(ESearchDir::B);
					SearchDirs.emplace_back(ESearchDir::R);
					SearchDirs.emplace_back(ESearchDir::RT);
					SearchDirs.emplace_back(ESearchDir::LB);
					break;
				case ESearchDir::B:
					SearchDirs.emplace_back(ESearchDir::B);
					SearchDirs.emplace_back(ESearchDir::RB);
					SearchDirs.emplace_back(ESearchDir::LB);
					break;
				case ESearchDir::LB:
					SearchDirs.emplace_back(ESearchDir::LB);
					SearchDirs.emplace_back(ESearchDir::B);
					SearchDirs.emplace_back(ESearchDir::L);
					SearchDirs.emplace_back(ESearchDir::LT);
					SearchDirs.emplace_back(ESearchDir::RB);
					break;
				case ESearchDir::L:
					SearchDirs.emplace_back(ESearchDir::L);
					SearchDirs.emplace_back(ESearchDir::LT);
					SearchDirs.emplace_back(ESearchDir::LB);
					break;
				case ESearchDir::LT:
					SearchDirs.emplace_back(ESearchDir::LT);
					SearchDirs.emplace_back(ESearchDir::T);
					SearchDirs.emplace_back(ESearchDir::L);
					SearchDirs.emplace_back(ESearchDir::LB);
					SearchDirs.emplace_back(ESearchDir::RT);
					break;
				default:
					break;
			}
			break;
		case Isometric:
			break;
	}
}

FNavNode* CNavigation::GetCornerRectT(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexY;

		if (IndexY >= CountY)
		{
			return {};
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
		{
			return {};
		}

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return {};
		}

		// 오른쪽이 막혀있고 오른쪽 위가 뚫려있을 경우
		int	CornerX = IndexX + 1;
		int	CornerY = IndexY;
		int	CornerIndex = 0;

		if (CornerX < CountX && CornerY + 1 < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 왼쪽이 막혀있고 왼쪽 위가 뚫려있을 경우
		CornerX = IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0 && CornerY + 1 < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}
	}

	return {};
}

FNavNode* CNavigation::GetCornerRectRT(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexX;
		++IndexY;

		if (IndexX >= CountX || IndexY >= CountY)
		{
			return nullptr;
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
			return nullptr;

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return nullptr;
		}

		// 왼쪽이 막혀있고 왼쪽 위가 뚫려있을 경우
		int	CornerX = IndexX - 1;
		int	CornerY = IndexY;
		int	CornerIndex = 0;

		if (CornerX >= 0 && CornerY + 1 < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 아래가 막혀있고 오른쪽 아래가 뚫려있을 경우
		CornerX = IndexX;
		CornerY = IndexY - 1;

		if (CornerX + 1 < CountX && CornerY >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + 1) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 위의 2가지 경우가 아니라면 오른쪽, 위 쪽으로 코너가 있는지
		// 체크해야 한다.
		FNavNode* FindNode = GetCornerRectR(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}

		FindNode = GetCornerRectT(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}
	}

	return nullptr;
}

FNavNode* CNavigation::GetCornerRectR(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexX;

		if (IndexX >= CountX)
		{
			return nullptr;
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
		{
			return nullptr;
		}

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return nullptr;
		}

		// 위가 막혀있고 오른쪽 위가 뚫려있을 경우
		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;
		int	CornerIndex = 0;

		if (CornerX + 1 < CountX && CornerY < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + 1) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 아래가 막혀있고 오른쪽 아래가 뚫려있을 경우
		CornerX = IndexX;
		CornerY = IndexY - 1;

		if (CornerX + 1 < CountX && CornerY >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + 1) == ETileType::Normal)
			{
				return Corner;
			}
		}
	}

	return nullptr;
}

FNavNode* CNavigation::GetCornerRectRB(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexX;
		--IndexY;

		if (IndexX >= CountX || IndexY < 0)
		{
			return nullptr;
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
		{
			return nullptr;
		}

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return nullptr;
		}

		// 위가 막혀있고 오른쪽 위가 뚫려있을 경우
		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;
		int	CornerIndex = 0;

		if (CornerX + 1 < CountX && CornerY < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + 1) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 왼쪽이 막혀있고 왼쪽 아래가 뚫려있을 경우
		CornerX = IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0 && CornerY - 1 >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 위의 2가지 경우가 아니라면 오른쪽, 아래 쪽으로 코너가 있는지
		// 체크해야 한다.
		FNavNode* FindNode = GetCornerRectR(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}

		FindNode = GetCornerRectB(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}
	}

	return nullptr;
}

FNavNode* CNavigation::GetCornerRectB(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexY;

		if (IndexY < 0)
		{
			return nullptr;
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
		{
			return nullptr;
		}

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return nullptr;
		}

		// 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우
		int	CornerX = IndexX + 1;
		int	CornerY = IndexY;
		int	CornerIndex = 0;

		if (CornerX < CountX && CornerY - 1 >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 왼쪽이 막혀있고 왼쪽 아래가 뚫려있을 경우
		CornerX = IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0 && CornerY - 1 >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}
	}

	return nullptr;
}

FNavNode* CNavigation::GetCornerRectLB(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexX;
		--IndexY;

		if (IndexX < 0 || IndexY < 0)
		{
			return nullptr;
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
		{
			return nullptr;
		}

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return nullptr;
		}

		// 위가 막혀있고 왼쪽 위가 뚫려있을 경우
		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;
		int	CornerIndex = 0;

		if (CornerX - 1 >= 0 && CornerY < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - 1) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우
		CornerX = IndexX + 1;
		CornerY = IndexY;

		if (CornerX < CountX && CornerY - 1 >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 위의 2가지 경우가 아니라면 왼쪽, 아래 쪽으로 코너가 있는지
		// 체크해야 한다.
		FNavNode* FindNode = GetCornerRectL(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}

		FindNode = GetCornerRectB(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}
	}

	return nullptr;
}

FNavNode* CNavigation::GetCornerRectL(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexX;

		if (IndexX < 0)
		{
			return nullptr;
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
		{
			return nullptr;
		}

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return nullptr;
		}

		// 위가 막혀있고 왼쪽 위가 뚫려있을 경우
		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;
		int	CornerIndex = 0;

		if (CornerX - 1 >= 0 && CornerY < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - 1) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 아래가 막혀있고 왼쪽 아래가 뚫려있을 경우
		CornerX = IndexX;
		CornerY = IndexY - 1;

		if (CornerX - 1 >= 0 && CornerY >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - 1) == ETileType::Normal)
			{
				return Corner;
			}
		}
	}

	return nullptr;
}

FNavNode* CNavigation::GetCornerRectLT(FNavNode* Node, FNavNode* EndNode)
{
	auto TileMap = this->TileMap.lock();

	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexX;
		++IndexY;

		if (IndexX < 0 || IndexY >= CountY)
		{
			return nullptr;
		}

		int	Index = IndexY * CountX + IndexX;

		FNavNode* Corner = &Nodes[Index];

		// 도착했으면 도착 노드를 바로 리턴한다.
		if (Corner == EndNode)
		{
			return Corner;
		}

		// 탐색하는 노드가 닫힌목록에 들어간 노드일 경우
		else if (Corner->NodeType == ENavNodeType::Close)
		{
			return nullptr;
		}

		// 이동 불가를 만났다면
		else if (TileMap->GetTileType(Index) == ETileType::UnableToMove)
		{
			return nullptr;
		}

		// 아래가 막혀있고 왼쪽 아래가 뚫려있을 경우
		int	CornerX = IndexX;
		int	CornerY = IndexY - 1;
		int	CornerIndex = 0;

		if (CornerX - 1 >= 0 && CornerY >= 0)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex - 1) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 오른쪽이 막혀있고 오른쪽 위가 뚫려있을 경우
		CornerX = IndexX + 1;
		CornerY = IndexY;

		if (CornerX < CountX && CornerY + 1 < CountY)
		{
			CornerIndex = CornerY * CountX + CornerX;

			if (TileMap->GetTileType(CornerIndex) == ETileType::UnableToMove
				&& TileMap->GetTileType(CornerIndex + CountX) == ETileType::Normal)
			{
				return Corner;
			}
		}

		// 위의 2가지 경우가 아니라면 왼쪽, 위 쪽으로 코너가 있는지
		// 체크해야 한다.
		FNavNode* FindNode = GetCornerRectL(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}

		FindNode = GetCornerRectT(Corner, EndNode);

		if (FindNode)
		{
			return Corner;
		}
	}

	return nullptr;
}

bool CNavigation::SortOpenNodes(const FNavNode* A, const FNavNode* B)
{
	return A->Total > B->Total;
}
