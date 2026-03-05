#include "CEnemy.h"

#include <CEngine.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <World/CWorld.h>

#include "CPlayerCharacter.h"
#include "../Strings.h"
#include "../Table/EnemyInfo.h"
#include "../Table/EnemyTable.h"
#include "../Table/MiscInfo.h"
#include "../Table/MiscTable.h"

bool CEnemy::Init()
{
	if (!CCharacter::Init())
	{
		return false;
	}

	auto Collider = this->Collider.lock();
	if (Collider)
	{
		Collider->SetCollisionProfile("Monster");

		Collider->SetOnCollisionBegin(this, &CEnemy::OnCollisionBegin);
	}

	SetTeam(ETeam::Enemy);

	Mesh = CreateComponent<CMeshComponent>(Key::Comp::Mesh);
	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(150, 150);
		if (Collider)
		{
			Collider->SetBoxExtent(Mesh->GetWorldScale().x, Mesh->GetWorldPosition().y);
		}

		Mesh->SetBlendState(0, "AlphaBlend");

		FMiscInfo* Misc;
		if (MiscTable::GetInst().TryGet(TableID(1), Misc))
		{
			CA2T FileName(Misc->PotatoBodyTexPath.c_str());
			Mesh->AddTexture(0, Misc->PotatoBodyTexPath, FileName, Key::Path::Brotato);
		}

		Mesh->SetInheritScale(true);
		Mesh->SetInheritRotation(true);

		Mesh->TrySetRenderLayer(ERenderOrder::CharacterBody);
	}

	return true;
}

void CEnemy::Update(const float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	if (Player.expired())
	{
		if (auto World = this->World.lock())
		{
			Player = World->FindObjectOfType<CPlayerCharacter>();
		}
	}

#pragma region AI
	auto Player = this->Player.lock();
	if (!Player)
	{
		return;
	}

	FEnemyInfo* Info;
	if (!EnemyTable::GetInst().TryGet(GetEnemyInfoID(), Info))
	{
		return;
	}

	auto Diff = Player->GetWorldPosition() - GetWorldPosition();
	float DistToPlayer = Diff.Length();
	auto ToPlayer = Diff.GetNormalized();

	auto Behaviors = Info->Behavior;

	if (Behaviors & EEnemyBehavior::Charge)
	{
		// 돌진 가능 여부 확인
		if (!bOnCharge)
		{
			if (DistToPlayer <= CHARGE_USE_DISTANCE)
			{
				if (ElapsedFromCharge >= ChargeCooldownTime)
				{
					if (ElapsedFromCharge == std::numeric_limits<float>::infinity())
					{
						ElapsedFromCharge = 0.f;
					}
					else
					{
						ElapsedFromCharge -= ChargeCooldownTime;
					}

					bOnCharge = true;
				}
			}
		}

		// 돌진 처리
		if (bOnCharge)
		{
			float DelMove = CHARGE_MOVE_SPEED * DeltaTime;
			DelMove = min(DelMove, CHARGE_MOVE_DISTANCE - ChargeMovedDist);

			auto MoveVec = ToPlayer * DelMove;
			AddWorldPosition(MoveVec);

			if (ChargeMovedDist >= CHARGE_MOVE_DISTANCE)
			{
				bOnCharge = false;
			}

			return;
		}
	}

	if (Behaviors & EEnemyBehavior::Fire)
	{

	}

	if (Behaviors & EEnemyBehavior::Kiting)
	{
		if (DistToPlayer <= CHARGE_USE_DISTANCE)
		{

		}
	}

	if (Behaviors & EEnemyBehavior::Chase)
	{
		float DelMove = MoveSpeed * DeltaTime;
		DelMove = min(DelMove, DistToPlayer);

		auto MoveVec = ToPlayer * DelMove;
		AddWorldPosition(MoveVec);
	}
#pragma endregion
}

float CEnemy::TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator)
{
	auto PC = std::dynamic_pointer_cast<CPlayerCharacter>(Instigator.lock());
	if (!PC)
	{
		return 0.f;
	}

	return 0.f;
}

void CEnemy::SetEnemyInfoID(const TableID& EnemyInfoID)
{
	this->EnemyInfoID = EnemyInfoID;

	FEnemyInfo* Info;
	if (!EnemyTable::GetInst().TryGet(GetEnemyInfoID(), Info))
	{
		return;
	}

	std::uniform_real_distribution<float> Dist(Info->MinSpeed, Info->MaxSpeed);
	MoveSpeed = Dist(CEngine::GetInst()->GetMT());

	// TODO: 난이도 영향 적용.
	//+ Info->HpIncrease * 난이도
	MaxHP = Info->HP;
	CurrHP = MaxHP;
}

CEnemy* CEnemy::Clone()
{
	return new CEnemy(*this);
}

void CEnemy::RunBehavior(const std::weak_ptr<CPlayerCharacter>& WeakPlayer)
{

}

void CEnemy::SetChargeCooldownTime(float& OutCooldownTime)
{
	FEnemyInfo* Info;
	if (!EnemyTable::GetInst().TryGet(GetEnemyInfoID(), Info))
	{
		return;
	}

	if ((Info->Behavior & EEnemyBehavior::Charge) == 0)
	{
		return;
	}

	std::uniform_real_distribution<float> Dist(2.5f, 3.5f);
	OutCooldownTime = Dist(CEngine::GetInst()->GetMT());
}

void CEnemy::OnCollisionBegin(const FVector& HitPoint, CCollider* Collider)
{
	FEnemyInfo* Info;
	if (!EnemyTable::GetInst().TryGet(GetEnemyInfoID(), Info))
	{
		return;
	}

	if (Info->Behavior & EEnemyBehavior::DamageOnTouch)
	{
		if (auto Player = std::dynamic_pointer_cast<CPlayerCharacter>(Collider->GetOwner().lock()))
		{
			// TODO: 난이도 영향 적용.
			//+ Info->DamageIncrease * 난이도
			Player->TakeDamage(Info->Damage, std::dynamic_pointer_cast<CGameObject>(shared_from_this()));
		}
	}
}
