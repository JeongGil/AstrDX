#include "CWeapon_Battle.h"

#include <CEngine.h>
#include <CTimer.h>
#include <Asset/Material/CMaterial.h>
#include <Component/CColliderBox2D.h>
#include <Component/CColliderSphere2D.h>
#include <Component/CMeshComponent.h>
#include <World/CWorld.h>

#include "CEnemy.h"
#include "CPlayerCharacter.h"
#include "CProjectile.h"
#include "../Strings.h"
#include "../Table/MiscTable.h"
#include "../Table/WeaponTable.h"


bool CWeapon_Battle::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Root = CreateComponent<CSceneComponent>(Key::Comp::Root);

	Collider = CreateComponent<CColliderBox2D>(Key::Comp::Collider, Key::Comp::Root);
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetRelativeRotationZ(90);
		Collider->SetDrawDebug(true);

		Collider->SetEnable(false);
		Collider->SetCollisionProfile("PlayerAttack");

		Collider->SetOnCollisionBegin<CWeapon_Battle>(this, &CWeapon_Battle::OnCollisionBegin);
		//Body->SetOnCollisionBegin<CBullet>(this, &CBullet::OnCollisionBegin);
	}

	Mesh = CreateComponent<CMeshComponent>(Key::Comp::Mesh, Key::Comp::Root);
	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->SetRelativeRotationZ(90);

		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");

		Mesh->SetBlendState(0, "AlphaBlend");

		Mesh->SetInheritScale(true);
		Mesh->SetInheritRotation(true);

		Mesh->TrySetRenderLayer(ERenderOrder::CharacterWeapon);
	}

	SearchCollider = CreateComponent<CColliderSphere2D>(Key::Comp::SearchCollider, Key::Comp::Root);
	if (auto Search = SearchCollider.lock())
	{
		Search->SetOnCollisionBlock<CWeapon_Battle>(this, &CWeapon_Battle::OnSearchCollisionBlock);
	}

	if (const auto* Misc = MiscTable::GetInst().Get())
	{
		MoveSpeed = static_cast<float>(Misc->WeaponMoveSpeed);
	}

	return true;
}

void CWeapon_Battle::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	auto Owner = this->Owner.lock();
	if (!Owner)
	{
		return;
	}

	const FWeaponInfo* WeaponInfo = WeaponTable::GetInst().Get(GetWeaponInfoID());
	if (!WeaponInfo)
	{
		return;
	}

	if (auto Anchor = PosAnchor.lock())
	{
		SetWorldPosition(Anchor->GetWorldPosition());
	}

	// 대상 조준
	if (auto Target = ClosestEnemy.lock())
	{
		float Degree = GetWorldPosition().GetViewTargetAngleDegree2D(Target->GetWorldRotation(), EAxis::Y);
		SetWorldRotationZ(Degree);
	}

	// 근접공격은 공격모션 종료 후 대기시간 감소.
	// 근접 무기 공격 중
	if (bOnMeleeAttack)
	{
		ElapsedMeleeMoveTime += DeltaTime;
	}
	// 원거리 무기 or 근접공격의 모션 종료
	else
	{
		ElapsedCooldownTime += DeltaTime * Owner->GetStat(EStat::AttackSpeed);
	}

	float CooldownSec = WeaponInfo->CooldownMS * 0.001f;
	if (ElapsedCooldownTime >= CooldownSec)
	{
		ElapsedCooldownTime = 0.f;

		// 근접공격은 따로
		if (WeaponInfo->bIsMeleeWeapon)
		{
			bOnMeleeAttack = true;
			ElapsedMeleeMoveTime = 0.f;
		}
		// TODO: 원거리 투사체 생성
		else
		{
			if (auto World = this->World.lock())
			{
				if (auto Projectile = World->CreateGameObject<CProjectile>("Projectile").lock())
				{
					FVector SpawnPos = GetWorldPosition() + TargetDir * BULLET_OFFSET;
					Projectile->SetWorldPosition(SpawnPos);

					Projectile->SetWorldRotation(GetWorldRotation());

					Projectile->SetOwnerWeapon(std::dynamic_pointer_cast<CWeapon_Battle>(shared_from_this()));
					Projectile->SetMoveDirection(TargetDir);
				}
			}
		}
	}

	// 근접 무기 공격중 무기 이동 처리.
	if (bOnMeleeAttack)
	{
		float TotalMoveTime = GetTotalMoveTime();
		float TotalDist = WeaponInfo->Range + Owner->GetStat(EStat::Range) * 0.5f;

		float Dist;
		// 찌르고
		if (ElapsedMeleeMoveTime < TotalMoveTime)
		{
			float Alpha = std::clamp(ElapsedMeleeMoveTime / TotalMoveTime, 0.f, 1.f);
			Dist = std::lerp(0.f, TotalDist, Alpha);
		}
		// 되돌아오기
		else
		{
			float Alpha = std::clamp((ElapsedMeleeMoveTime - TotalMoveTime) / TotalMoveTime, 0.f, 1.f);
			Dist = std::lerp(TotalDist, 0.f, Alpha);
		}

		FVector RelativePos = TargetDir * Dist;
		auto Anchor = PosAnchor.lock();

		SetWorldPosition(Anchor->GetWorldPosition() + RelativePos);

		// 근접 공격 종료
		if (ElapsedMeleeMoveTime >= 2 * TotalMoveTime)
		{
			bOnMeleeAttack = false;

			ElapsedCooldownTime = ElapsedMeleeMoveTime - 2 * TotalMoveTime;
			ElapsedMeleeMoveTime = 0.f;

			TargetDir = FVector::Zero;
			MovedDistance = 0.f;
		}
	}
}

void CWeapon_Battle::PostUpdate(const float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	// 오브젝트의 PostUpdate이후 충돌 처리됨.
	// 그 전에 초기화.
	ClosestSqrDist = FLT_MAX;
	ClosestEnemy.reset();
	if (const FWeaponInfo* Info = WeaponTable::GetInst().Get(GetWeaponInfoID()))
	{
		if (auto Search = SearchCollider.lock())
		{
			Search->SetRadius(CalcAttackRange(Info, Owner));
		}
	}
}

CWeapon_Battle* CWeapon_Battle::Clone()
{
	return new CWeapon_Battle(*this);
}

void CWeapon_Battle::OnProjectileCollideOnMonster(const std::weak_ptr<CEnemy>& WeakMonster)
{
	auto Monster = WeakMonster.lock();
	if (!Monster)
	{
		return;
	}

	FWeaponInfo* Info;
	if (!WeaponTable::GetInst().TryGet(GetWeaponInfoID(), Info))
	{
		return;
	}

	auto Character = Owner.lock();
	if (!Character)
	{
		return;
	}

	auto [Damage, bIsCrit] = CalcAttackDamage(Info, Owner);

	Monster->TakeDamage(Damage, Owner);
}

void CWeapon_Battle::InitWeaponInfo(TableID ID)
{
	FWeaponInfo* Info;
	if (!WeaponTable::GetInst().TryGet(ID, Info))
	{
		return;
	}

	if (auto Mesh = this->Mesh.lock())
	{
		auto WPath = std::wstring(Info->SpritePath.begin(), Info->SpritePath.end());
		auto WMatTexInfo = Mesh->AddTexture(0, Info->SpritePath, WPath.c_str(), Key::Path::Brotato);
		if (auto MatTexInfo = WMatTexInfo.lock())
		{
			if (auto Texture = MatTexInfo->Texture.lock())
			{
#pragma warning(push)
#pragma warning(disable: 4244)
				const FTextureInfo* TexInfo = Texture->GetTexture();
				Mesh->SetWorldScale(TexInfo->Width, TexInfo->Height);

				if (Info->bIsMeleeWeapon)
				{
					if (auto Col = Collider.lock())
					{
						Col->SetBoxExtent(TexInfo->Width, TexInfo->Height);

						Col->SetEnable(true);
					}
				}
#pragma warning(pop)
			}
		}
	}
}

std::weak_ptr<CSceneComponent> CWeapon_Battle::GetClosestEnemy()
{
	if (auto World = this->World.lock())
	{
		auto Enemies = World->FindObjectsOfType<CEnemy>();

		float SqrDist = std::numeric_limits<float>::infinity();
		std::weak_ptr<CEnemy> Closest;

		auto EnemyView = Enemies
			| std::views::transform([](const auto& Weak) { return Weak.lock(); })
			| std::views::filter([](const auto& Enemy) { return Enemy != nullptr; });

		for (const auto& Enemy : EnemyView)
		{
			float CurrSqrDist = (GetWorldPosition() - Enemy->GetWorldPosition()).SqrLength();
			if (CurrSqrDist < SqrDist)
			{
				SqrDist = CurrSqrDist;
				Closest = Enemy;
			}
		}

		if (auto Target = Closest.lock())
		{
			return Target->GetRootComponent();
		}
	}

	return {};
}

int CWeapon_Battle::CalcAttackRange(const FWeaponInfo* WeaponInfo,
	const std::weak_ptr<CPlayerCharacter>& PlayerCharacter)
{
	auto PC = PlayerCharacter.lock();
	if (!WeaponInfo || !PC)
	{
		return 0.f;
	}

	return WeaponInfo->Range + static_cast<int>(PC->GetStat(EStat::Range));
}

FAttackResult CWeapon_Battle::CalcAttackDamage(const FWeaponInfo* WeaponInfo,
	const std::weak_ptr<CPlayerCharacter>& PlayerCharacter)
{
	auto PC = PlayerCharacter.lock();
	if (!WeaponInfo || !PC)
	{
		return {};
	}

	float Damage = WeaponInfo->BaseDamage;
	float Added{};
	FDamageScale Scaler{};

	Scaler = WeaponInfo->DamageScale1;
	if (Scaler.StatType != EStat::None)
	{
		Added = PC->GetStat(Scaler.StatType) * Scaler.ScalePercent * 0.01f;
		Damage += Added;
	}

	Scaler = WeaponInfo->DamageScale2;
	if (Scaler.StatType != EStat::None)
	{
		Added = PC->GetStat(Scaler.StatType) * Scaler.ScalePercent * 0.01f;
		Damage += Added;
	}

	Scaler = WeaponInfo->DamageScale3;
	if (Scaler.StatType != EStat::None)
	{
		Added = PC->GetStat(Scaler.StatType) * Scaler.ScalePercent * 0.01f;
		Damage += Added;
	}

	auto MT = CEngine::GetInst()->GetMT();
	std::uniform_real_distribution<float> Dist(0.f, 100.f);
	float Dice = Dist(MT);
	bool bIsCrit = Dice < (WeaponInfo->CritChancePercent + PC->GetStat(EStat::CritChance)) * 0.01f;
	if (bIsCrit)
	{
		Damage *= WeaponInfo->CritDamagePercent * 0.01f;
	}

	Damage *= (100 + PC->GetStat(EStat::Damage)) * 0.01f;

	return { Damage, bIsCrit };
}

void CWeapon_Battle::OnCollisionBegin(const FVector& HitPoint, CCollider* Other)
{
	auto ColObj = Other->GetOwner().lock();
	if (!ColObj)
	{
		return;
	}

	auto ColChar = std::dynamic_pointer_cast<CCharacter>(ColObj);
	if (!ColChar || ColChar->GetTeam() != ETeam::Enemy)
	{
		return;
	}

	FWeaponInfo* WeaponInfo;
	if (!WeaponTable::GetInst().TryGet(WeaponInfoID, WeaponInfo))
	{
		return;
	}

	auto [Damage, bIsCrit] = CalcAttackDamage(WeaponInfo, Owner);

	ColChar->TakeDamage(Damage, Owner);

	if (ColChar && ColChar->GetAlive() && ColChar->GetEnable())
	{
		// TODO: 넉백
		FVector Dir = (ColChar->GetWorldPosition() - HitPoint).GetNormalized();
	}
}

void CWeapon_Battle::OnSearchCollisionBlock(const FVector& HitPoint, CCollider* Other)
{
	if (Other == nullptr)
	{
		return;
	}

	FVector Direction = Other->GetWorldPosition() - GetWorldPosition();
	auto SqrDist = Direction.SqrLength();
	if (SqrDist < ClosestSqrDist)
	{
		ClosestSqrDist = SqrDist;
		ClosestEnemy = Other->GetOwner();
		TargetDir = Direction.GetNormalized();
	}
}

float CWeapon_Battle::GetTotalMoveTime() const
{
	if (MoveSpeed == 0.f)
	{
		return {};
	}

	FWeaponInfo* Info;
	if (!WeaponTable::GetInst().TryGet(GetWeaponInfoID(), Info))
	{
		return {};
	}

	if (!Info->bIsMeleeWeapon)
	{
		return {};
	}

	auto Owner = this->Owner.lock();
	if (!Owner)
	{
		return {};
	}

	return Owner->GetStat(EStat::Range) / MoveSpeed;
}
