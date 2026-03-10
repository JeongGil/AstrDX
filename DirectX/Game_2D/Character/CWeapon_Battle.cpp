#include "CWeapon_Battle.h"

#include <CEngine.h>
#include <cmath>
#include <CTimer.h>
#include <unordered_set>
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

		Collider->SetOnCollisionBegin<CWeapon_Battle>(this, &CWeapon_Battle::OnCollisionBeginOverlap);
		//Body->SetOnCollisionBegin<CBullet>(this, &CBullet::OnCollisionBeginOverlap);
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

		Mesh->SetRenderLayer(ERenderOrder::CharacterWeapon);
	}

	SearchCollider = CreateComponent<CColliderSphere2D>(Key::Comp::SearchCollider, Key::Comp::Root);
	if (auto Search = SearchCollider.lock())
	{
		Search->SetOnCollisionBegin<CWeapon_Battle>(this, &CWeapon_Battle::OnSearchCollisionBeginOverlap);
		Search->SetOnCollisionEnd<CWeapon_Battle>(this, &CWeapon_Battle::OnSearchCollisionEndOverlap);
		Search->SetCollisionProfile("FindEnemy");
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

	SortCloseEnemies();

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

	// Anchor가 설정되어 있으면 항상 Anchor 위치를 따라감
	if (auto Anchor = PosAnchor.lock())
	{
		// 근접 무기 공격중 무기 이동 처리가 아닐 때만 Anchor 위치로 고정
		if (!bOnMeleeAttack)
		{
			SetWorldPosition(Anchor->GetWorldPosition());
		}
	}

	// 대상 조준 (근접 공격 중이 아닐 때만 방향 업데이트)
	if (!bOnMeleeAttack)
	{
		if (auto Target = GetClosestEnemy().lock())
		{
			float Degree = GetWorldPosition().GetViewTargetAngleDegree2D(Target->GetWorldPosition(), EAxis::Y);
			SetWorldRotationZ(Degree);

			TargetDir = (Target->GetWorldPosition() - GetWorldPosition()).GetNormalized();
		}
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

	float TotalDist = GetTotalRange();

	float CooldownSec = WeaponInfo->CooldownMS * 0.001f;
	if (ElapsedCooldownTime >= CooldownSec
		&& TotalDist >= GetClosestDistance())
	{
		ElapsedCooldownTime = 0.f;

		// 근접공격은 따로
		if (WeaponInfo->bIsMeleeWeapon)
		{
			bOnMeleeAttack = true;
			ElapsedMeleeMoveTime = 0.f;
			// 새로운 공격 시작 시 이전 공격의 히트 기록 초기화
			HitEnemiesInCurrentAttack.clear();
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
		float TotalMoveTime = GetTotalMoveTime(TotalDist);

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

		if (Anchor)
		{
			SetWorldPosition(Anchor->GetWorldPosition() + RelativePos);
		}

		// 근접 공격 종료
		if (ElapsedMeleeMoveTime >= 2 * TotalMoveTime)
		{
			bOnMeleeAttack = false;

			ElapsedCooldownTime = ElapsedMeleeMoveTime - 2 * TotalMoveTime;
			ElapsedMeleeMoveTime = 0.f;

			TargetDir = FVector::Zero;
			MovedDistance = 0.f;

			// 공격 종료 시 히트 기록 초기화
			HitEnemiesInCurrentAttack.clear();
		}
	}
}

void CWeapon_Battle::PostUpdate(const float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	// 오브젝트의 PostUpdate이후 충돌 처리됨.
	// 그 전에 Range변화를 반영.

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

void CWeapon_Battle::OnCollisionBeginOverlap(const FVector& HitPoint, CCollider* Other)
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

	// 현재 공격에서 이미 맞은 적인지 확인 (중복 데미지 방지)
	if (HitEnemiesInCurrentAttack.contains(Other))
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

	// 이 적을 현재 공격의 히트 목록에 추가
	HitEnemiesInCurrentAttack.insert(Other);

	if (ColChar && ColChar->GetAlive() && ColChar->GetEnable())
	{
		// TODO: 넉백
		//FVector Dir = (ColChar->GetWorldPosition() - HitPoint).GetNormalized();
	}
}

void CWeapon_Battle::OnSearchCollisionBeginOverlap(const FVector& HitPoint, CCollider* Other)
{
	if (Other == nullptr)
	{
		return;
	}

	CloseEnemies.emplace_back(Other->GetOwner());
}

void CWeapon_Battle::OnSearchCollisionEndOverlap(CCollider* Other)
{
	if (Other == nullptr)
	{
		return;
	}

	std::erase_if(CloseEnemies, [Target = Other->GetOwner()](const std::weak_ptr<CGameObject>& Item)
		{
			// 1. 이미 파괴되었거나, 2. 찾는 대상인 경우 제거
			return Item.expired() || IsSameTarget(Item, Target);
		});
}

float CWeapon_Battle::GetTotalRange() const
{
	FWeaponInfo* Info;
	if (!WeaponTable::GetInst().TryGet(GetWeaponInfoID(), Info))
	{
		return {};
	}

	auto Owner = this->Owner.lock();
	if (!Owner)
	{
		return {};
	}

	auto WeaponRange = Info->Range;

	auto OwnerRange = Owner->GetStat(EStat::Range);
	if (Info->bIsMeleeWeapon)
	{
		OwnerRange *= 0.f;
	}

	return WeaponRange + OwnerRange;
}

float CWeapon_Battle::GetTotalMoveTime(float Range) const
{
	if (MoveSpeed == 0.f)
	{
		return {};
	}

	return Range / MoveSpeed;
}

void CWeapon_Battle::SortCloseEnemies()
{
	std::erase_if(CloseEnemies, [](const std::weak_ptr<CGameObject>& WeakObject)
		{
			auto Object = WeakObject.lock();
			if (!Object || !Object->GetAlive() || !Object->GetEnable())
			{
				return true;
			}

			return false;
		});

	const FVector Pos = GetWorldPosition();
	std::ranges::sort(CloseEnemies, [Pos](const std::weak_ptr<CGameObject>& A, const std::weak_ptr<CGameObject>& B)
		{
			return (A.lock()->GetWorldPosition() - Pos).SqrLength() < (B.lock()->GetWorldPosition() - Pos).SqrLength();
		});

	if (CloseEnemies.empty())
	{
		ClosestDistance = std::numeric_limits<float>::infinity();
		ClosestEnemy.reset();
	}
	else
	{
		ClosestEnemy = CloseEnemies[0];
		ClosestDistance = (ClosestEnemy.lock()->GetWorldPosition() - GetWorldPosition()).Length();
	}
}

float CWeapon_Battle::GetClosestDistance() const
{
	return ClosestDistance;
}

const std::weak_ptr<CGameObject>& CWeapon_Battle::GetClosestEnemy() const
{
	if (CloseEnemies.empty())
	{
		return {};
	}

	return *CloseEnemies.begin();
}
