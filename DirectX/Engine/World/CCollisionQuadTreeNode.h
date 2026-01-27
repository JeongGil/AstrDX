#pragma once

#include "../EngineInfo.h"

class CShader;
class CCBufferTransform;
class CCollisionQuadTree;
class CCollider;
class CWorld;
class CMesh;

constexpr int QUADTREE_DIVISION_COUNT = 3;
constexpr int QUADTREE_DEPTH_MAX = 4;

class CCollisionQuadTreeNode : public std::enable_shared_from_this<CCollisionQuadTreeNode>
{
	friend CCollisionQuadTree;

private:
	CCollisionQuadTreeNode();

public:
	~CCollisionQuadTreeNode();

private:
	std::weak_ptr<CWorld> World;

	std::weak_ptr<::CCollisionQuadTree> Tree;
	std::weak_ptr<CCollisionQuadTreeNode> Parent;
	std::shared_ptr<CCollisionQuadTreeNode> Children[4];

	FVector Center;
	FVector Size;
	int Depth = 0;

	std::list<std::weak_ptr<CCollider>> Colliders;

#ifdef _DEBUG

	std::shared_ptr<CCBufferTransform> TransformCBuffer;

#endif

public:
	void AddCollider(const std::weak_ptr<CCollider>& Collider, std::vector<std::shared_ptr<CCollisionQuadTreeNode>>& NodePool);
	void Collide(const float DeltaTime);
	bool CollideMouse(std::weak_ptr<CCollider>& Result, const float DeltaTime, const FVector2& MousePos);
	void Render(const std::weak_ptr<CMesh>& Mesh, const std::weak_ptr<CShader>& Shader);
	void ReturnNodePool(std::vector<std::shared_ptr<CCollisionQuadTreeNode>>& NodePool);

private:
	bool IsInCollider(const std::weak_ptr<CCollider>& Collider);
	bool IsInCollider(const FVector2& MousePos);
	void CreateChild(std::vector<std::shared_ptr<CCollisionQuadTreeNode>>& NodePool);
	static bool SortMouseCollision(const std::weak_ptr<CCollider>& A, const std::weak_ptr<CCollider>& B);

public:
	void SetWorld(const std::weak_ptr<CWorld>& World)
	{
		this->World = World;
	}

	void SetTree(const std::weak_ptr<CCollisionQuadTree>& Tree)
	{
		this->Tree = Tree;
	}
};

