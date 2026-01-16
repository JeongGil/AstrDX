#include "CPlayerCharacter.h"

#include <Component/CMeshComponent.h>
#include <Component/CObjectMovementComponent.h>
#include <World/CWorld.h>

#include "../Strings.h"
#include "../Table/MiscTable.h"

#include <atlbase.h>
#include <atlconv.h>

bool CPlayerCharacter::Init()
{
	if (!CCharacter::Init())
	{
		return false;
	}

	Potato = CreateComponent<CMeshComponent>(Key::Comp::Potato);
	if (auto Body = this->Potato.lock())
	{
		Body->SetShader("DefaultTexture2D");
		Body->SetMesh("RectTex");
		Body->SetWorldScale(150, 150);
		
		Body->SetMaterialBaseColor(0, FColor::White);
		Body->SetBlendState(0, "AlphaBlend");

		FMiscInfo* Misc;
		if (MiscTable::Instance().TryGet(TableID(1), Misc))
		{
			CA2T FileName(Misc->PotatoBodyTexPath.c_str());
			Body->AddTexture(0, Misc->PotatoBodyTexPath, FileName, Key::Path::Brotato);
		}

		Body->SetInheritScale(true);
		Body->SetInheritRotation(true);
	}

	Leg = CreateComponent<CMeshComponent>(Key::Comp::Leg, Key::Comp::Potato);
	if (auto Leg = this->Leg.lock())
	{
		Leg->SetShader("DefaultTexture2D");
		Leg->SetMesh("RectTex");
		Leg->SetWorldScale(100, 50);
		Leg->SetRelativePosition(FVector(0, -25, 0));

		Leg->SetMaterialBaseColor(0, FColor::White);
		Leg->SetBlendState(0, "AlphaBlend");

		Leg->SetInheritScale(true);
		Leg->SetInheritRotation(true);
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
			Input->SetBindFunction<CPlayerCharacter>(Key::Input::MoveUp, EInputType::Hold, this, &CPlayerCharacter::MoveDown);

			Input->AddBindKey(Key::Input::MoveLeft, 'A');
			Input->SetBindFunction<CPlayerCharacter>(Key::Input::MoveUp, EInputType::Hold, this, &CPlayerCharacter::MoveLeft);

			Input->AddBindKey(Key::Input::MoveRight, 'D');
			Input->SetBindFunction<CPlayerCharacter>(Key::Input::MoveUp, EInputType::Hold, this, &CPlayerCharacter::MoveRight);
		}
	}

	return true;
}

void CPlayerCharacter::Update(const float DeltaTime)
{
	CCharacter::Update(DeltaTime);
}

void CPlayerCharacter::PostUpdate(const float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CPlayerCharacter::SetCharacterVisual(const FCharacterVisualInfo& VisualInfo)
{
	CharacterVisualInfo = VisualInfo;

	for (const auto& DecoPath : VisualInfo.Decos)
	{
		if (!DecoPath.empty())
		{
			CA2T Path(DecoPath.c_str());
			auto WeakDeco = CreateComponent<CMeshComponent>(Key::Comp::Deco, Key::Comp::Potato);
			if (auto Deco = WeakDeco.lock())
			{
				Deco->SetShader("DefaultTexture2D");
				Deco->SetMesh("RectTex");
				Deco->SetWorldScale(150, 150);

				Deco->SetMaterialBaseColor(0, FColor::White);
				Deco->SetBlendState(0, "AlphaBlend");

				CA2T FileName(DecoPath.c_str());
				Deco->AddTexture(0, DecoPath, FileName, Key::Path::Brotato);
			}
		}
	}
}

void CPlayerCharacter::SetWeapon(const std::weak_ptr<CWeapon_Inventory>& Weapon, size_t SlotIdx)
{
	assert(SlotIdx < INVENTORY_MAX_WEAPON);

	if (auto World = this->World.lock())
	{
		auto WeakWeapon = World->CreateGameObject<CWeapon_Battle>(Key::Obj::Weapon);
		if (auto Weapon = WeakWeapon.lock())
		{
			// TODO: Set Weapon status.
			Weapons.push_back(Weapon);
		}
	}
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
