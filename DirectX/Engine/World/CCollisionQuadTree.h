#pragma once

#include "CCollisionQuadTreeNode.h"
#include "../EngineInfo.h"

class CCBufferCollider;
class CMesh;
class CShader;
class CWorld;
class CWorldCollision;

class CCollisionQuadTree : public std::enable_shared_from_this<CCollisionQuadTree>
{
	friend CWorldCollision;

private:
	CCollisionQuadTree();

public:
	~CCollisionQuadTree();

private:
	std::weak_ptr<CWorld> World;

	std::shared_ptr<CCollisionQuadTreeNode> Root;
	std::vector<std::weak_ptr<CCollisionQuadTreeNode>> CollisionNodes;

	std::vector<std::shared_ptr<CCollisionQuadTreeNode>> NodePool;

#ifdef _DEBUG

	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::shared_ptr<CCBufferCollider> ColliderCBuffer;

#endif

public:
	void AddCollisionNode(const std::weak_ptr<CCollisionQuadTreeNode>& Node);
	void EraseCollisionNode(const std::weak_ptr<CCollisionQuadTreeNode>& Node);

	void SetWorld(const std::weak_ptr<CWorld>& World)
	{
		this->World = World;
	}

public:
	bool Init();
	void AddCollider(const std::weak_ptr<CCollider>& Collider);
	void Update(const float DeltaTime);
	void Collide(const float DeltaTime);
	void Render();
	void ReturnNodePool();

private:
	void UpdateInfo();
};

