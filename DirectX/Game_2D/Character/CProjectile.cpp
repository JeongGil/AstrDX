#include "CProjectile.h"

#include <Component/CAnimation2DComponent.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <Component/CProjectileMovementComponent.h>
#include <World/CWorld.h>

#include "CEnemy.h"
#include "CPlayerCharacter.h"
#include "CWeapon_Battle.h"
#include "../Table/EnemyTable.h"
#include "../Table/ProjectileTable.h"

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
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	Animation = CreateComponent<CAnimation2DComponent>("Animation");

	Collider = CreateComponent<CColliderBox2D>("Collider");
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetOnCollisionBlock<CProjectile>(this, &CProjectile::OnCollisionMapBoundary);

		Collider->SetInheritScale(false);
		Collider->SetEnable(true);

		Collider->SetOnCollisionBegin(this, &CProjectile::OnCollisionBegin);

#if defined(_DEBUG) || defined(DEBUG)
		Collider->SetDrawDebug(true);
#endif
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
		 if (auto Monster = std::dynamic_pointer_cast<CEnemy>(Other->GetOwner().lock()))
		 {
			 OwnerWeapon->OnProjectileCollideOnMonster(HitPoint, Other);
		 }
	}
	// 무기 없으면 몬스터의 공격
	else if (auto OwnerCharacter = this->OwnerCharacter.lock())
	{
		auto Monster = std::dynamic_pointer_cast<CEnemy>(OwnerCharacter);
		if (!Monster)
		{
			return;
		}

		Monster->OnProjectileHitPC(HitPoint, Other);
	}

	if (RemainPenetration == 0)
	{
		Destroy();
		return;
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

void CProjectile::SetProjectileID(const TableID& NewID)
{
	ProjectileID = NewID;

	FProjectileInfo* Info{};
	if (!ProjectileTable::GetInst().TryGet(ProjectileID, Info) || !Info || Info->Name.empty())
	{
		return;
	}

	if (auto Anim = Animation.lock())
	{
		if (auto Mesh = this->Mesh.lock())
		{
			Anim->SetUpdateComponent(Mesh);
		}

		Anim->AddAnimation(Info->Name);
		Anim->SetLoop(Info->Name, true);
		Anim->ReplayAnimation(Info->Name);
	}

	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			if (auto Texture = WorldAssetMgr->FindTexture(Info->Name).lock())
			{
				if (auto Mesh = this->Mesh.lock())
				{
					const FTextureInfo* TexInfo = Texture->GetTexture();
					const float MeshSizeX = static_cast<float>(TexInfo->Width) * 0.7f;
					const float MeshSizeY = static_cast<float>(TexInfo->Height) * 0.7f;

					Mesh->SetWorldScale(MeshSizeX, MeshSizeY);

					if (auto Col = Collider.lock())
					{
						Col->SetBoxExtent(MeshSizeX * 0.7f, MeshSizeY * 0.7f);
						Col->SetWorldPosition(Mesh->GetWorldPosition());
					}
				}
			}
		}
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
