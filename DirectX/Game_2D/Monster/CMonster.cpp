#include "CMonster.h"

#include "../Player/CBullet.h"
#include "World/CWorld.h"
#include "Component/CMeshComponent.h"

bool CMonster::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("Color2D");
		Mesh->SetMesh("CenterRectColor");
		Mesh->SetRelativeScale(100, 100);
	}

	if (auto World = this->World.lock())
	{
		FireTarget = World->FindObject<CGameObject>("Player");
	}

	return true;
}

void CMonster::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	FireCoolTime -= DeltaTime;
	if (FireCoolTime <= 0.f)
	{
		FireCoolTime += 1.f;

		if (auto World = this->World.lock())
		{
			static int Counter = 0;
			const std::string BulletName = "MonsterBullet_";
			auto WeakBullet = World->CreateGameObject<CBullet>(BulletName + std::to_string(Counter++));

			if (auto Bullet = WeakBullet.lock())
			{
				FVector Position = GetWorldPosition() + GetAxis(EAxis::Y) * 75;
				Bullet->SetWorldPosition(Position);
				Bullet->SetWorldRotation(GetWorldRotation());

				if (auto Target = FireTarget.lock())
				{
					FVector Dir = Target->GetWorldPosition() - Position;
					Dir.Normalize();

					Bullet->SetMoveDirection(Dir);
				}
			}
		}
	}
}
