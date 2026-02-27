#include "CPlayerCharacter.h"

#include <atlbase.h>
#include <atlconv.h>
#include <CDevice.h>
#include <CEngine.h>
#include <Asset/Material/CMaterial.h>
#include <Component/CCameraComponent.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <Component/CObjectMovementComponent.h>
#include <Component/CSceneComponent.h>
#include <World/CWorld.h>

#include "CEnemy.h"
#include "CWeapon_Battle.h"
#include "../Strings.h"
#include "../Inventory/CInventoryItem_Weapon.h"
#include "../Table/CharacterVisualTable.h"
#include "../Table/MiscTable.h"

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

	Potato = CreateComponent<CMeshComponent>(Key::Comp::Potato, Key::Comp::Root);
	if (auto Body = this->Potato.lock())
	{
		Body->SetShader("DefaultTexture2D");
		Body->SetMesh("CenterRectTex");
		Body->SetWorldScale(150, 150);
		if (auto Col = Collider.lock())
		{
			Col->SetBoxExtent(Body->GetWorldScale().x, Body->GetWorldPosition().y);
		}

		Body->SetBlendState(0, "AlphaBlend");

		FMiscInfo* Misc;
		if (MiscTable::GetInst().TryGet(TableID(1), Misc))
		{
			CA2T FileName(Misc->PotatoBodyTexPath.c_str());
			Body->AddTexture(0, Misc->PotatoBodyTexPath, FileName, Key::Path::Brotato);
		}

		Body->SetInheritScale(true);
		Body->SetInheritRotation(true);

		Body->TrySetRenderLayer(ERenderOrder::CharacterBody);
	}

	Leg = CreateComponent<CMeshComponent>(Key::Comp::Leg, Key::Comp::Potato);
	if (auto Leg = this->Leg.lock())
	{
		Leg->SetShader("DefaultTexture2D");
		Leg->SetMesh("CenterRectTex");
		Leg->SetWorldScale(100, 50);
		Leg->SetRelativePosition(FVector(0, -25, 0));

		Leg->SetBlendState(0, "AlphaBlend");

		FMiscInfo* Misc;
		if (MiscTable::GetInst().TryGet(TableID(1), Misc))
		{
			CA2T FileName(Misc->PotatoLegTexPath.c_str());
			Leg->AddTexture(0, Misc->PotatoLegTexPath, FileName, Key::Path::Brotato);
		}

		Leg->SetInheritScale(true);
		Leg->SetInheritRotation(true);

		Leg->TrySetRenderLayer(ERenderOrder::CharacterLeg);
	}

	MovementComponent = CreateComponent<CObjectMovementComponent>(Key::Comp::ObjMove);
	if (auto Move = MovementComponent.lock())
	{
		Move->SetUpdateComponent(Root);
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
	}

	for (auto& WeaponAnchor : WeaponAnchors)
	{
		auto WAnchor = CreateComponent<CSceneComponent>(Key::Comp::Anchor, Key::Comp::Root);
		if (auto Anchor = WAnchor.lock())
		{
			WeaponAnchor = Anchor;
		}
	}

	RemainAbsorbAttackStack = GetStat(EStat::AbsorbAttack);

	return true;
}

void CPlayerCharacter::Update(const float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	if (auto MoveCmp = MovementComponent.lock())
	{
		if (MoveCmp->GetMoveDirection().x > 0.f)
		{
			auto Meshes = GetComponents<CMeshComponent>();
			for (const auto& WMesh : Meshes)
			{
				if (auto Mesh = WMesh.lock())
				{
					// TODO: Add a shader by referring to CShaderTexture2D.Apply Symmetric.
					//Mesh->
				}
			}
		}
		else if (MoveCmp->GetMoveDirection().x < 0.f)
		{
			// TODO: Refer to the above.
		}
	}
}

void CPlayerCharacter::PostUpdate(const float DeltaTime)
{
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

	return Damage * GetArmoredDmgRatio(GetStat(EStat::Armor));
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

			Deco->TrySetRenderLayer(ERenderOrder::CharacterDeco);
		}
	}
}

void CPlayerCharacter::SetAnchorPosition(size_t WeaponCount)
{
	assert(WeaponCount == Weapons.size());

	const auto& AnchorPoses = AnchorPositions.at(WeaponCount);
	for (size_t i = 0; i < WeaponCount; i++)
	{
		WeaponAnchors[i]->SetRelativePosition(AnchorPoses[i]);
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
		}
	}

	SetAnchorPosition(Weapons.size());
}

int CPlayerCharacter::GetStat(EStat::Type StatType)
{
	return 0;
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
	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(FVector::Axis[EAxis::Y]);
	}
}

void CPlayerCharacter::MoveDown()
{
	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(-FVector::Axis[EAxis::Y]);
	}
}

void CPlayerCharacter::MoveLeft()
{
	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(-FVector::Axis[EAxis::X]);
	}
}

void CPlayerCharacter::MoveRight()
{
	if (auto Move = MovementComponent.lock())
	{
		Move->AddMove(FVector::Axis[EAxis::X]);
	}
}
