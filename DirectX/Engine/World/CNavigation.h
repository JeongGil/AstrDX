#pragma once

#include "../EngineInfo.h"

class CTileMapComponent;
class CWorld;
class CThreadNavigation;

namespace ESearchDir
{
	enum Type
	{
		T,
		RT,
		R,
		RB,
		B,
		LB,
		L,
		LT,
		End
	};
}

enum class ENavNodeType
{
	None,
	Open,
	Close
};

struct FNavNode
{
	FNavNode* Parent = nullptr;
	ENavNodeType NodeType = ENavNodeType::None;
	
	FVector2 Pos;
	FVector2 Size;
	FVector2 Center;

	int IndexX = 0;
	int IndexY = 0;
	int Index = 0;

	float Dist = FLT_MAX;
	float Heuristic = FLT_MAX;
	float Total = FLT_MAX;

	std::list<ESearchDir::Type> SearchDirs;
};

class CNavigation
{
	friend CWorld;
	friend CThreadNavigation;

private:
	CNavigation();

public:
	~CNavigation();

private:
	std::weak_ptr<CTileMapComponent> TileMap;
	std::vector<FNavNode> Nodes;
	ETileShape Shape = ETileShape::Rect;

	int CountX = 0;
	int CountY = 0;
	float DiagonalCost = 0.f;
	FVector2 TileSize;

	std::vector<FNavNode*> OpenNodes;
	std::vector<FNavNode*> UsedNodes;

public:
	void SetTileMap(const std::weak_ptr<CTileMapComponent>& TileMap);
	bool FindPath(const FVector& Start, const FVector& End, std::list<FVector>& Paths);

private:
	bool FindNode(FNavNode* Node, FNavNode* EndNode, const FVector& End, std::list<FVector>& Paths);

	FNavNode* GetCorner(ESearchDir::Type Dir, FNavNode* Node, FNavNode* EndNode);
	void AddDir(ESearchDir::Type Dir, FNavNode* Node);
	FNavNode* GetCornerRectT(FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectRT(FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectR(FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectRB(FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectB(FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectLB(FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectL(FNavNode* Node, FNavNode* EndNode);
	FNavNode* GetCornerRectLT(FNavNode* Node, FNavNode* EndNode);

	static bool SortOpenNodes(const FNavNode* A, const FNavNode* B);
};

