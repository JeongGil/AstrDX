#include "CWeapon_Battle.h"

#include <CTimer.h>
#include <Asset/Material/CMaterial.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <World/CWorld.h>
#include <Component/CColliderSphere2D.h>
#include <CEngine.h>

#include "CEnemy.h"
#include "CPlayerCharacter.h"
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

	const FWeaponInfo* WeaponInfo = WeaponTable::GetInst().Get(GetWeaponInfoID());

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
	// 원거리 무기 or 근접공격의 모션 종료
	if (!bOnMeleeAttack)
	{
		ElapsedCooldownTime += DeltaTime;
	}

	if (WeaponInfo != nullptr)
	{
		float CooldownSec = WeaponInfo->CooldownMS * 0.001f;
		if (ElapsedCooldownTime >= CooldownSec)
		{
			if (ElapsedCooldownTime == std::numeric_limits<float>::infinity())
			{
				ElapsedCooldownTime = 0.f;
			}
			else
			{
				ElapsedCooldownTime -= CooldownSec;
			}

			// 근접공격은 따로
			if (WeaponInfo->bIsMeleeWeapon)
			{
				bOnMeleeAttack = true;
				ElapsedMeleeMovingTime = 0.f;
			}
			// 원거리 투사체
			else
			{
				
			}
		}
	}

	// 근접 무기 공격중 무기 이동 처리.
	if (bOnMeleeAttack)
	{
		
	}


	//	// Attack
	//	if (auto InvenWeapon = Origin.lock())
	//	{
	//		FWeaponInfo* Info;
	//		if (WeaponTable::GetInst().TryGet(InvenWeapon->GetWeaponInfoID(), Info))
	//		{
	//			using Clock = std::chrono::steady_clock;

	//			auto Now = CTimer::Now();
	//			auto Elapsed = Now - LastFiredTime;
	//			auto Cooldown = std::chrono::duration_cast<Clock::duration>(std::chrono::duration<float>(Info->CooldownMS * 0.001f));

	//			if (Elapsed >= Cooldown)
	//			{
	//				if ((GetWorldPosition() - Target->GetWorldPosition()).SqrLength() <= Info->Range * Info->Range)
	//				{
	//					// Fire
	//					auto Overflow = Elapsed - Cooldown;
	//					LastFiredTime = Now - Overflow;


	//				}
	//			}
	//		}
	//	}
	//}

	//if (auto InvenWeapon = Origin.lock())
	//{
	//	FWeaponInfo* Info;
	//	if (WeaponTable::GetInst().TryGet(InvenWeapon->GetWeaponInfoID(), Info))
	//	{
	//		//float RTT = Info.ran
	//	}
	//}
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

				if (auto Col = Collider.lock())
				{
					Col->SetBoxExtent(TexInfo->Width, TexInfo->Height);
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
		auto NPCs = World->FindObjectsOfType<CEnemy>();

		float SqrDist = FLT_MAX;
		std::weak_ptr<CEnemy> Closest;
		for (const auto& WNPC : NPCs)
		{
			if (auto NPC = WNPC.lock())
			{
				float CurrSqrDist = (GetWorldPosition() - NPC->GetWorldPosition()).SqrLength();
				if (CurrSqrDist < SqrDist)
				{
					SqrDist = CurrSqrDist;
					Closest = WNPC;
				}
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

float CWeapon_Battle::CalcAttackDamage(const FWeaponInfo* WeaponInfo,
	const std::weak_ptr<CPlayerCharacter>& PlayerCharacter)
{
	auto PC = PlayerCharacter.lock();
	if (!WeaponInfo || !PC)
	{
		return 0.f;
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
	if (Dice < WeaponInfo->CritChancePercent * 0.01f)
	{
		Damage *= WeaponInfo->CritDamagePercent * 0.01f;
	}

	return Damage;
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

	int InputDamage{};

	FWeaponInfo* WeaponInfo;
	if (WeaponTable::GetInst().TryGet(WeaponInfoID, WeaponInfo))
	{
		InputDamage = CalcAttackDamage(WeaponInfo, Owner);
	}

	ColChar->TakeDamage(InputDamage, Owner);

	if (ColChar && ColChar->GetAlive() && ColChar->GetEnable())
	{
		// TODO: 넉백
	}
}

void CWeapon_Battle::OnSearchCollisionBlock(const FVector& HitPoint, CCollider* Other)
{
	if (Other == nullptr)
	{
		return;
	}

	auto SqrDist = (GetWorldPosition() - Other->GetWorldPosition()).SqrLength();
	if (SqrDist < ClosestSqrDist)
	{
		ClosestSqrDist = SqrDist;
		ClosestEnemy = Other->GetOwner();
	}
}
