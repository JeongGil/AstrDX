#include "CPlayerCharacter.h"

#include <atlbase.h>
#include <atlconv.h>
#include <CDevice.h>
#include <CEngine.h>
#include <Asset/Material/CMaterial.h>
#include <Component/CAnimation2DComponent.h>
#include <Component/CCameraComponent.h>
#include <Component/CColliderBox2D.h>
#include <Component/CColliderSphere2D.h>
#include <Component/CMeshComponent.h>
#include <Component/CObjectMovementComponent.h>
#include <Component/CSceneComponent.h>
#include <World/CWorld.h>

#include "CCameraObject.h"
#include "CDropItem.h"
#include "CEnemy.h"
#include "CWeapon_Battle.h"
#include "../Strings.h"
#include "../Inventory/CInventoryData.h"
#include "../Inventory/CInventoryItem_Weapon.h"
#include "../Table/CharacterBaseTable.h"
#include "../Table/CharacterVisualTable.h"
#include "../Table/WeaponSetBonusTable.h"
#include "../World/CBrotatoWorld_Battle.h"

bool CPlayerCharacter::Init()
{
	if (!CCharacter::Init())
	{
		return false;
	}

	if (auto Col = Collider.lock())
	{
		Col->SetCollisionProfile("Player");
	}

	SetTeam(ETeam::Player);

	const auto* CharacterBase = CharacterBaseTable::GetInst().Get();
	Potato = CreateComponent<CMeshComponent>(Key::Comp::Potato, Key::Comp::Root);
	PotatoAnim = CreateComponent<CAnimation2DComponent>(Key::Anim::Potato, Key::Comp::Root);
	if (auto Body = this->Potato.lock())
	{
		Body->SetShader("DefaultTexture2D");
		Body->SetMesh("CenterRectTex");
		Body->SetWorldScale(150, 150);
		if (auto Col = Collider.lock())
		{
			Col->SetBoxExtent(CharacterBase->HitColliderSizeX, CharacterBase->HitColliderSizeY);

#if defined(_DEBUG) || defined(DEBUG)
			Col->SetDrawDebug(true);
#endif

		}

		Body->SetBlendState(0, "AlphaBlend");

		CA2T FileName(CharacterBase->PotatoBodyTexPath.c_str());
		Body->AddTexture(0, CharacterBase->PotatoBodyTexPath, FileName, Key::Path::Brotato);

		Body->SetInheritScale(true);
		Body->SetInheritRotation(true);

		Body->SetRenderLayer(ERenderOrder::CharacterBody);

		SleepOnSpawnComponents.push_back(Potato);
		Body->SetEnable(false);

		if (auto Anim = PotatoAnim.lock())
		{
			Anim->SetUpdateComponent(Potato);

			Anim->AddAnimation(CharacterBase->PotatoBodyTexPath);
			Anim->SetLoop(CharacterBase->PotatoBodyTexPath, true);

			SleepOnSpawnComponents.push_back(PotatoAnim);
			Anim->SetEnable(false);
		}
	}

	Leg = CreateComponent<CMeshComponent>(Key::Comp::Leg, Key::Comp::Potato);
	LegAnim = CreateComponent<CAnimation2DComponent>(Key::Anim::Leg, Key::Comp::Potato);
	if (auto Leg = this->Leg.lock())
	{
		Leg->SetShader("DefaultTexture2D");
		Leg->SetMesh("CenterRectTex");
		Leg->SetWorldScale(100, 50);
		Leg->SetRelativePosition(FVector(0, -25, 0));

		Leg->SetBlendState(0, "AlphaBlend");

		CA2T FileName(CharacterBase->PotatoLegTexPath.c_str());
		Leg->AddTexture(0, CharacterBase->PotatoLegTexPath, FileName, Key::Path::Brotato);

		Leg->SetInheritScale(true);
		Leg->SetInheritRotation(true);

		Leg->SetRenderLayer(ERenderOrder::CharacterLeg);

		SleepOnSpawnComponents.push_back(this->Leg);
		Leg->SetEnable(false);

		if (auto Anim = LegAnim.lock())
		{
			Anim->SetUpdateComponent(Leg);

			Anim->AddAnimation(CharacterBase->PotatoLegTexPath);
			Anim->SetLoop(CharacterBase->PotatoLegTexPath, true);

			SleepOnSpawnComponents.push_back(LegAnim);
			Anim->SetEnable(false);
		}
	}

	MovementComponent = CreateComponent<CObjectMovementComponent>(Key::Comp::ObjMove);
	if (auto Move = MovementComponent.lock())
	{
		Move->SetUpdateComponent(Root);

		SleepOnSpawnComponents.push_back(Move);
		Move->SetEnable(false);
	}

	if (auto World = this->World.lock())
	{
		if (auto Input = World->GetInput().lock())
		{
			Input->AddBindKey(Key::Input::MoveUp, 'W');
			Input->SetBindFunction<CPlayerCharacter>(Key::Input::MoveUp, EInputType::Hold, this, &CPlayerCharacter::MoveUp);

			Input->AddBindKey(Key::Input::MoveDown, 'S');
			Input->SetBindFunction<CPlayerCharacter>(Key::Input::MoveDown, EInputType::Hold, this, &CPlayerCharacter::MoveDown);

			Input->AddBindKey(Key::Input::MoveLeft, 'A');
			Input->SetBindFunction<CPlayerCharacter>(Key::Input::MoveLeft, EInputType::Hold, this, &CPlayerCharacter::MoveLeft);

			Input->AddBindKey(Key::Input::MoveRight, 'D');
			Input->SetBindFunction<CPlayerCharacter>(Key::Input::MoveRight, EInputType::Hold, this, &CPlayerCharacter::MoveRight);
		}
	}

	Camera = CreateComponent<CCameraComponent>(Key::Comp::PlayerCamera);
	if (auto Cam = Camera.lock())
	{
		const auto& Resolution = CDevice::GetInst()->GetResolution();
		Cam->SetProjection(ECameraProjectionType::Orthogonal,
			90.f, static_cast<float>(Resolution.Width), static_cast<float>(Resolution.Height), 1000);

		Cam->SetInheritRotation(false);		

		SleepOnSpawnComponents.push_back(Camera);
		Cam->SetEnable(false);

		if (auto World = this->World.lock())
		{
			World->GetCameraManager().lock()->SetMainCamera(Cam);
			if (auto BrotatoWorld = std::dynamic_pointer_cast<CBrotatoWorld_Battle>(World))
			{
				if (auto SubCam = BrotatoWorld->GetSubCameraObj().lock())
				{
					SubCam->SetAnchor(Cam);
				}
			}
		}
	}

	for (auto& WeaponAnchor : WeaponAnchors)
	{
		auto WAnchor = CreateComponent<CSceneComponent>(Key::Comp::Anchor, Key::Comp::Root);
		WeaponAnchor = WAnchor;
	}

	PickupCollider = CreateComponent<CColliderSphere2D>("PickupRange");
	if (auto Col = PickupCollider.lock())
	{
		Col->SetCollisionProfile("PickupRange");
		Col->SetOnCollisionBegin<CPlayerCharacter>(this, &CPlayerCharacter::OnPickupColliderBeginOverlap);

		Col->SetDrawDebug(true);

		SleepOnSpawnComponents.push_back(PickupCollider);
		Col->SetEnable(false);
	}

	RemainAbsorbAttackStack = static_cast<int>(GetStat(EStat::AbsorbAttack));

	return true;
}

void CPlayerCharacter::Update(const float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	auto CharacterBase = CharacterBaseTable::GetInst().Get();
	if (auto Col = PickupCollider.lock())
	{
		float RangeRatio = 1 + GetStat(EStat::PickupRange) * 0.01f;
		Col->SetRadius(CharacterBase->BasePickupRange * RangeRatio);
	}
}

void CPlayerCharacter::PostUpdate(const float DeltaTime)
{
	if (auto MoveCmp = MovementComponent.lock())
	{
		const FVector& MoveDir = MoveCmp->GetMoveDirection();

		if (!MoveDir.IsZero() && MoveDir.x != 0.f)
		{
			bool bSymmetry = MoveDir.x < 0.f;

			if (bSymmetry != bLastSymmetry)
			{
				bLastSymmetry = bSymmetry;

				auto CharacterBase = CharacterBaseTable::GetInst().Get();

				if (auto Anim = PotatoAnim.lock())
				{
					Anim->SetSymmetry(CharacterBase->PotatoBodyTexPath, bSymmetry);
				}

				if (auto Anim = LegAnim.lock())
				{
					Anim->SetSymmetry(CharacterBase->PotatoLegTexPath, bSymmetry);
				}

				auto VisualInfo = CharacterVisualTable::GetInst().Get(CharacterVisualInfoID);

				for (size_t i = 0; i < DecoAnims.size(); i++)
				{
					if (auto DecoAnim = DecoAnims[i].lock())
					{
						std::string DecoPath;
						if (i == 0)
						{
							DecoPath = VisualInfo->Eye;
						}
						else if (i == 1)
						{
							DecoPath = VisualInfo->Mouth;
						}
						else if (i - 2 < VisualInfo->Decos.size())
						{
							DecoPath = VisualInfo->Decos[i - 2];
						}

						if (!DecoPath.empty())
						{
							DecoAnim->SetSymmetry(DecoPath, bSymmetry);
						}
					}
				}
			}
		}
	}

	CCharacter::PostUpdate(DeltaTime);
}

float CPlayerCharacter::TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator)
{
	auto Enemy = std::dynamic_pointer_cast<CEnemy>(Instigator.lock());
	if (!Enemy)
	{
		return 0.f;
	}

	auto MT = CEngine::GetInst()->GetMT();
	std::uniform_real_distribution<float> Dist(0.f, 100.f);
	auto Dice = Dist(MT);
	if (Dice < GetStat(EStat::Dodge) * 0.01f)
	{
		// TODO: Generate Dodge event.

		return 0.f;
	}

	if (RemainAbsorbAttackStack > 0)
	{
		--RemainAbsorbAttackStack;

		return 0.f;
	}

	// Round enemy damage before apply armor.
	Damage = round(Damage);

	SetCurrHP(GetCurrHP() - Damage);

	return Damage * GetArmoredDmgRatio(static_cast<int>(GetStat(EStat::Armor)));
}

void CPlayerCharacter::CreateDeco(const std::string& DecoPath)
{
	if (!DecoPath.empty())
	{
		CA2T Path(DecoPath.c_str());
		auto WeakDeco = CreateComponent<CMeshComponent>(Key::Comp::Deco, Key::Comp::Potato);
		if (auto Deco = WeakDeco.lock())
		{
			Deco->SetShader("DefaultTexture2D");
			Deco->SetMesh("CenterRectTex");
			Deco->SetRelativePosition(0, 0);

			Deco->SetBlendState(0, "AlphaBlend");

			Deco->SetInheritScale(true);
			Deco->SetInheritRotation(true);

			CA2T FileName(DecoPath.c_str());
			auto WMatTexInfo = Deco->AddTexture(0, DecoPath, FileName, Key::Path::Brotato);
			if (auto MatTexInfo = WMatTexInfo.lock())
			{
				if (auto Texture = MatTexInfo->Texture.lock())
				{
					const FTextureInfo* TexInfo = Texture->GetTexture();
#pragma warning(push)
#pragma warning(disable: 4244)
					Deco->SetWorldScale(TexInfo->Width, TexInfo->Height);
#pragma warning(pop)
				}
			}

			Deco->SetRenderLayer(ERenderOrder::CharacterDeco);

			Decos.push_back(Deco);

			auto WeakDecoAnim = CreateComponent<CAnimation2DComponent>(Key::Anim::Deco, Key::Comp::Potato);
			if (auto DecoAnim = WeakDecoAnim.lock())
			{
				DecoAnim->SetUpdateComponent(Deco);

				DecoAnim->AddAnimation(DecoPath);
				DecoAnim->SetLoop(DecoPath, true);

				DecoAnims.push_back(DecoAnim);
			}
		}
	}
}

void CPlayerCharacter::SetAnchorPosition(size_t WeaponCount)
{
	assert(WeaponCount == Weapons.size());

	const auto& AnchorPoses = AnchorPositions.at(WeaponCount);
	for (size_t i = 0; i < WeaponCount; i++)
	{
		if (auto WeaponAnchor = WeaponAnchors[i].lock())
		{
			WeaponAnchor->SetRelativePosition(AnchorPoses[i]);
		}
	}
}

float CPlayerCharacter::GetArmoredDmgRatio(int Armor)
{
	float fArmor = static_cast<float>(Armor);

	if (Armor > 0)
	{
		return 1.f / (1.f + fArmor / 15.f);
	}

	if (Armor < 0)
	{
		return (15.f - 2.f * fArmor) / (15.f - fArmor);
	}

	return 1.f;
}

void CPlayerCharacter::OnPickupColliderBeginOverlap(const FVector& HitPoint, CCollider* Other)
{
	if (Other == nullptr)
	{
		return;
	}

	auto DropItem = std::dynamic_pointer_cast<CDropItem>(Other->GetOwner().lock());
	if (!DropItem)
	{
		return;
	}

	DropItem->SetIsCollecting(true);
}

void CPlayerCharacter::SetCharacterVisual(TableID VisualInfoID)
{
	CharacterVisualInfoID = VisualInfoID;

	FCharacterVisualInfo* VisualInfo;
	if (!CharacterVisualTable::GetInst().TryGet(VisualInfoID, VisualInfo))
	{
		return;
	}

	CreateDeco(VisualInfo->Eye);
	CreateDeco(VisualInfo->Mouth);

	for (const auto& DecoPath : VisualInfo->Decos)
	{
		CreateDeco(DecoPath);
	}
}

void CPlayerCharacter::AddWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon)
{
	assert(Weapons.size() < INVENTORY_MAX_WEAPON);
	auto InvenWeapon = Weapon.lock();
	if (!InvenWeapon)
	{
		return;
	}

	if (auto World = this->World.lock())
	{
		auto WeakWeapon = World->CreateGameObject<CWeapon_Battle>(Key::Obj::Weapon);
		if (auto WeaponObj = WeakWeapon.lock())
		{
			Weapons.push_back(WeaponObj);

			WeaponObj->SetOwner(std::dynamic_pointer_cast<CPlayerCharacter>(shared_from_this()));
			WeaponObj->SetWeapon(InvenWeapon);

			SetAnchorPosition(Weapons.size());
			WeaponObj->SetPosAnchor(WeaponAnchors[Weapons.size() - 1]);

			WeaponObj->AddRelativeRotationZ(90);

			WeaponObj->SetWeaponInfoID(InvenWeapon->GetWeaponInfoID());
		}
	}

	SetAnchorPosition(Weapons.size());
}

float CPlayerCharacter::GetStat(EStat::Type StatType)
{
	float Value{};

	if (BaseStats.contains(StatType))
	{
		Value += BaseStats[StatType];
	}

	if (UpgradeStats.contains(StatType))
	{
		Value += UpgradeStats[StatType];
	}

	// Weapon Set Bonus.
	for (const auto& [WeaponType, TypeCount] : CInventoryData::GetInst().GetWeaponTypeCounts())
	{
		FWeaponSetBonusInfo* Info;

		TableID ID(static_cast<int>(WeaponType));
		if (WeaponSetBonusTable::GetInst().TryGet(ID, Info))
		{
			auto Bonus = Info->Bonus1[TypeCount - 1];
			if (Bonus.StatType == StatType)
			{
				Value += Bonus.StatValue;
			}

			Bonus = Info->Bonus2[TypeCount - 1];
			if (Bonus.StatType == StatType)
			{
				Value += Bonus.StatValue;
			}
		}
	}

	// TODO: 오라.

	return Value;
}

int CPlayerCharacter::GetToolTipDmgReductionPercent(int Armor)
{
	return static_cast<int>(round((1.f - GetArmoredDmgRatio(Armor)) * 100.f));
}

CPlayerCharacter* CPlayerCharacter::Clone()
{
	return new CPlayerCharacter(*this);
}

void CPlayerCharacter::MoveUp()
{
	if (IsPendingDead())
	{
		return;
	}

	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(FVector::Axis[EAxis::Y]);
	}
}

void CPlayerCharacter::MoveDown()
{
	if (IsPendingDead())
	{
		return;
	}

	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(-FVector::Axis[EAxis::Y]);
	}
}

void CPlayerCharacter::MoveLeft()
{
	if (IsPendingDead())
	{
		return;
	}

	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(-FVector::Axis[EAxis::X]);
	}
}

void CPlayerCharacter::MoveRight()
{
	if (IsPendingDead())
	{
		return;
	}

	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(FVector::Axis[EAxis::X]);
	}
}

void CPlayerCharacter::OnDead()
{
	CCharacter::OnDead();

	if (auto BrotatoWorld = std::dynamic_pointer_cast<CBrotatoWorld_Battle>(World.lock()))
	{
		if (auto CamMgr =  BrotatoWorld->GetCameraManager().lock())
		{
			if (auto SubCam = BrotatoWorld->GetSubCameraObj().lock())
			{
				CamMgr->SetMainCamera(SubCam->GetCameraComponent());
			}
		}
	}
}

void CPlayerCharacter::UpdateSpawnSequence(const float DeltaTime)
{
	CCharacter::UpdateSpawnSequence(DeltaTime);
}
