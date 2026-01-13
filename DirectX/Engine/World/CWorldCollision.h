#pragma once

#include "../EngineInfo.h"

class CCollider;

class CWorldCollision
{
	friend class CWorld;

public:
	bool Init();
	void Update(const float DeltaTime);

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
	std::list<std::weak_ptr<CCollider>> Colliders;

	float Interval = 0.f;
	float Elapsed = 0.f;

private:
	CWorldCollision() = default;

public:
	~CWorldCollision() = default;
};

