#pragma once

#include "../EngineInfo.h"
#include "CCollisionQuadTree.h"

class CCollider;
class CWorld;

class CWorldCollision
{
	friend CWorld;

public:
	bool Init();
	void Update(const float DeltaTime);
	void Render();
	void ReturnNodePool();

	void SetWorld(const std::weak_ptr<CWorld>& World)
	{
		this->World = World;
		QuadTree->SetWorld(World);
	}

	void SetInterval(float Interval)
	{
		this->Interval = Interval;
	}
	void AddCollider(const std::weak_ptr<CCollider>& Collider)
	{
		Colliders.push_back(Collider);
	}

private:
	void UpdateInfo();

private:
	std::weak_ptr<CWorld> World;
	std::shared_ptr<CCollisionQuadTree> QuadTree;
	std::list<std::weak_ptr<CCollider>> Colliders;

	float Interval = 0.f;
	float Elapsed = 0.f;

private:
	CWorldCollision() = default;

public:
	~CWorldCollision() = default;
};

