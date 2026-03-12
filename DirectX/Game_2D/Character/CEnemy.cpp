#include "CEnemy.h"

#include <CEngine.h>
#include <Component/CAnimation2DComponent.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <World/CWorld.h>

#include "CDropItem.h"
#include "CPlayerCharacter.h"
#include "../Strings.h"
#include "../Utility.h"
#include "../Table/EnemyInfo.h"
#include "../Table/EnemyTable.h"
#include "../Table/MiscInfo.h"
#include "../Table/MiscTable.h"
#include "../World/CBrotatoWorld_Battle.h"

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
		Collider->SetDrawDebug(true);
	}

	SetTeam(ETeam::Enemy);

	auto Misc = MiscTable::GetInst().Get();

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

		// Init에서는 텍스처를 추가하지 않음 - SetEnemyInfoID에서 설정됨
		// CA2T FileName(Misc->PotatoBodyTexPath.c_str());
		// Mesh->AddTexture(0, Misc->PotatoBodyTexPath, FileName, Key::Path::Brotato);

		Mesh->SetInheritScale(true);
		Mesh->SetInheritRotation(true);

		Mesh->TrySetRenderLayer(ERenderOrder::CharacterBody);
	}

	Animation = CreateComponent<CAnimation2DComponent>(Key::Anim::Enemy);
	if (auto Anim = Animation.lock())
	{
		Anim->SetUpdateComponent(Mesh);
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
					ElapsedFromCharge = 0.f;

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
		if (ElapsedFromFire >= FIRE_COOLDOWN_TIME)
		{
			ElapsedFromFire = 0.f;
		}
	}

	if (Behaviors & EEnemyBehavior::Kiting)
	{
		if (DistToPlayer <= CHARGE_USE_DISTANCE)
		{
			float DelMove = MoveSpeed * DeltaTime;

			auto MoveVec = -ToPlayer * DelMove;
			AddWorldPosition(MoveVec);

			return;
		}
	}

	if (Behaviors & EEnemyBehavior::Chase)
	{
		float DelMove = MoveSpeed * DeltaTime;
		DelMove = min(DelMove, DistToPlayer);

		auto MoveVec = ToPlayer * DelMove;
		AddWorldPosition(MoveVec);

		return;
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

	AddCurrHP(-Damage);

	return Damage;
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

	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->ClearTextures(0);
		
		CA2T FileName(Info->SpritePath.c_str());
		auto MatTexInfo = Mesh->AddTexture(0, Info->SpritePath, FileName, Key::Path::Brotato);
		SetMeshSizeFromTexture(MatTexInfo, this->Mesh);

		if (auto Anim = Animation.lock())
		{
			Anim->AddAnimation(Info->SpritePath);
			Anim->SetLoop(Info->SpritePath, true);
		}
	}

	if (auto Col = Collider.lock())
	{
		Col->SetBoxExtent(Info->ColliderSize);
	}
}

void CEnemy::SetCurrHP(const float CurrHP)
{
	this->CurrHP = std::clamp(CurrHP, 0.f, MaxHP);

	if (this->CurrHP == 0.f)
	{
		OnDead();
	}
}

CEnemy* CEnemy::Clone()
{
	return new CEnemy(*this);
}

void CEnemy::OnDead()
{
	CCharacter::OnDead();

	auto Info = EnemyTable::GetInst().Get(GetEnemyInfoID());

	auto World = std::dynamic_pointer_cast<CBrotatoWorld_Battle>(this->World.lock());
	if (!World)
	{
		return;
	}

	auto Player = this->Player.lock();
	if (!Player)
	{
		return;
	}

	if (Info->Material > 0)
	{
		if (auto DropMaterial = World->CreateGameObject<CDropItem>(Key::Obj::Material).lock())
		{
			DropMaterial->SetItemType(EDropItemType::Material);
			DropMaterial->SetMaterialCount(Info->Material);

			DropMaterial->SetWorldPosition(GetWorldPosition());
		}
	}

	// 확정 전설
	if (Info->Type != EEnemyType::Normal)
	{
		if (auto ItemBox = World->CreateGameObject<CDropItem>(Key::Obj::LegendaryItemBox).lock())
		{
			ItemBox->SetItemType(EDropItemType::LegendaryItemBox);
			ItemBox->SetWorldPosition(GetWorldPosition());
		}
	}
	else
	{
		std::uniform_real_distribution<float> Dist(0.f, 100.f);

		float LuckCorrection = (100 + Player->GetStat(EStat::Luck)) * 0.01f;

		auto ConsumableDropChance = Info->ConsumableDropPercent * LuckCorrection;
		auto Dice1 = Dist(CEngine::GetInst()->GetMT());
		
		if (ConsumableDropChance > Dice1)
		{
			auto ItemBoxDropChance = Info->CrateDropPercent * LuckCorrection / (1 + World->GetItemBoxDropCount());
			auto Dice2 = Dist(CEngine::GetInst()->GetMT());

			if (ItemBoxDropChance > Dice2)
			{
				if (auto ItemBox = World->CreateGameObject<CDropItem>(Key::Obj::ItemBox).lock())
				{
					ItemBox->SetItemType(EDropItemType::ItemBox);
					ItemBox->SetWorldPosition(GetWorldPosition());
				}
			}
			else
			{
				if (auto Fruit = World->CreateGameObject<CDropItem>(Key::Obj::Fruit).lock())
				{
					Fruit->SetItemType(EDropItemType::Fruit);
					Fruit->SetWorldPosition(GetWorldPosition());
				}
			}
		}
	}
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
