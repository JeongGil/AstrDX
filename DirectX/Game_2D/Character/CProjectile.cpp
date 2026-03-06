#include "CProjectile.h"

#include <Component/CAnimation2DComponent.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <Component/CProjectileMovementComponent.h>

#include "CEnemy.h"
#include "CPlayerCharacter.h"
#include "CWeapon_Battle.h"
#include "../Table/EnemyTable.h"

bool CProjectile::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Mesh = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("RectTex");
	}

	Animation = CreateComponent<CAnimation2DComponent>("Animation");
	if (auto Anim = Animation.lock())
	{

	}

	Collider = CreateComponent<CColliderBox2D>("Collider");
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetOnCollisionBlock<CProjectile>(this, &CProjectile::OnCollisionMapBoundary);

		Collider->SetInheritScale(false);
		Collider->SetEnable(true);

		Collider->SetDrawDebug(true);
	}

	Movement = CreateComponent<CProjectileMovementComponent>("Movement");
	if (auto Move = Movement.lock())
	{
		Move->SetRangeFunction<CProjectile>(this, &CProjectile::Destroy);
		Move->SetUpdateComponent(Mesh);
	}

	return true;
}

CProjectile* CProjectile::Clone()
{
	return new CProjectile(*this);
}

void CProjectile::SetEnableMove(bool bEnable) const
{
	if (auto Move = Movement.lock())
	{
		Move->SetEnable(bEnable);
	}
}

void CProjectile::SetCollisionProfile(const std::string& ProfileKey) const
{
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetCollisionProfile(ProfileKey);
	}
}

void CProjectile::SetMoveDirection(const FVector& Direction) const
{
	if (auto Move = Movement.lock())
	{
		Move->SetMoveDirection(Direction);
	}
}

void CProjectile::OnCollisionBegin(const FVector& HitPoint, CCollider* Other)
{
	// 공격자가 플레이어
	if (auto OwnerWeapon = this->OwnerWeapon.lock())
	{
		

		auto Monster = std::dynamic_pointer_cast<CEnemy>(Other->GetOwner().lock());
		if (!Monster)
		{
			return;
		}

		//Monster->TakeDamage(TODO, Player);
	}
	// 무기 없으면 몬스터의 공격
	else if (auto OwnerCharacter = this->OwnerCharacter.lock())
	{
		auto Monster = std::dynamic_pointer_cast<CEnemy>(OwnerCharacter);
		if (!Monster)
		{
			return;
		}

		auto Player = std::dynamic_pointer_cast<CPlayerCharacter>(Other->GetOwner().lock());
		if (!Player)
		{
			return;
		}

		FEnemyInfo* Info;
		if (!EnemyTable::GetInst().TryGet(Monster->GetEnemyInfoID(), Info))
		{
			return;
		}

		// TODO: DamageIncrease 적용.
		Player->TakeDamage(Info->Damage, Monster);
	}

	if (RemainPenetration == 0)
	{
		Destroy();
	}

	--RemainPenetration;
}

void CProjectile::OnCollisionMapBoundary(const FVector& HitPoint, CCollider* Other)
{
	if (auto Move = Movement.lock())
	{
		Move->SetLifeTime(3.f);
	}
}

void CProjectile::SetOwnerWeapon(const std::weak_ptr<CWeapon_Battle>& OwnerWeapon)
{
	this->OwnerWeapon = OwnerWeapon;
	if (auto Weapon = this->OwnerWeapon.lock())
	{
		SetOwnerCharacter(std::dynamic_pointer_cast<CCharacter>(Weapon->GetOwner().lock()));
	}
}
