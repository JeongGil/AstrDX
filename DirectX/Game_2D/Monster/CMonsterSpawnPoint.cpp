#include "CMonsterSpawnPoint.h"

#include <World/CWorld.h>

#include "CMonster.h"

bool CMonsterSpawnPoint::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	auto cmp = CreateComponent<CSceneComponent>("Root");
	if (cmp.expired())
	{
		return false;
	}

	return true;
}

void CMonsterSpawnPoint::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (Spawned.expired())
	{
		ElapsedFromSpawn += DeltaTime;

		if (ElapsedFromSpawn >= SpawnTime)
		{
			ElapsedFromSpawn = 0.f;

			auto WeakCDO = CObject::FindCDO(SpawnType);
			auto MonsterCDO = std::dynamic_pointer_cast<CMonster>(WeakCDO.lock());

			if (auto World = this->World.lock())
			{
				auto WeakSpawned = World->CreateCloneGameObject<CMonster>("Monster", MonsterCDO);
				if (auto Spawned = WeakSpawned.lock())
				{
					Spawned->SetWorldPosition(GetWorldPosition());
					Spawned->SetWorldRotation(GetWorldRotation());
					Spawned->SetSimulatePhysics(true);
					Spawned->SetUseGravity(true);

					this->Spawned = Spawned;
				}
			}
		}
	}
}
