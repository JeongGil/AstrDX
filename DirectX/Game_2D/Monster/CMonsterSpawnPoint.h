#pragma once
#include "Object/CGameObject.h"

class CMonster;

class CMonsterSpawnPoint :
    public CGameObject
{
	friend class CWorld;
	friend CObject;

public:
	template <typename T>
	void SetSpawnType()
	{
		SpawnType = typeid(T);
	}

	void SetSpawnTime(const float SpawnTime)
	{
		this->SpawnTime = SpawnTime;
	}

private:
	std::type_index SpawnType = typeid(void);
	float ElapsedFromSpawn = 0.f;
	float SpawnTime = 0.f;

	std::weak_ptr<CMonster> Spawned;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CMonsterSpawnPoint() = default;
	CMonsterSpawnPoint(const CMonsterSpawnPoint& other) = default;
	CMonsterSpawnPoint(CMonsterSpawnPoint&& other) noexcept = default;
	CMonsterSpawnPoint& operator=(const CMonsterSpawnPoint& other) = default;
	CMonsterSpawnPoint& operator=(CMonsterSpawnPoint&& other) noexcept = default;

public:
	~CMonsterSpawnPoint() override = default;
};

