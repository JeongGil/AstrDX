#include "CPlayerCharacter.h"

#include <Component/CMeshComponent.h>
#include <World/CWorld.h>

#include "../Strings.h"
#include "../Table/MisnTable.h"

#include <atlbase.h>
#include <atlconv.h>

bool CPlayerCharacter::Init()
{
	if (!CCharacter::Init())
	{
		return false;
	}

	Potato = CreateComponent<CMeshComponent>(Comp::Potato);
	if (auto Body = this->Potato.lock())
	{
		Body->SetShader("DefaultTexture2D");
		Body->SetMesh("RectTex");
		Body->SetWorldScale(150, 150);
		
		Body->SetMaterialBaseColor(0, FColor::White);
		Body->SetBlendState(0, "AlphaBlend");

		FMiscInfo Misc;
		if (MiscTable::Instance().TryGet(TableID(1), Misc))
		{
			CA2T FileName(Misc.PotatoBodyTexPath.c_str());
			Body->AddTexture(0, Misc.PotatoBodyTexPath, FileName, Path::Brotato);
		}

		Body->SetInheritScale(true);
		Body->SetInheritRotation(true);
	}

	Leg = CreateComponent<CMeshComponent>(Comp::Leg, Comp::Potato);
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
			auto WeakDeco = CreateComponent<CMeshComponent>(Comp::Deco, Comp::Potato);
			if (auto Deco = WeakDeco.lock())
			{
				Deco->SetShader("DefaultTexture2D");
				Deco->SetMesh("RectTex");
				Deco->SetWorldScale(150, 150);

				Deco->SetMaterialBaseColor(0, FColor::White);
				Deco->SetBlendState(0, "AlphaBlend");

				CA2T FileName(DecoPath.c_str());
				Deco->AddTexture(0, DecoPath, FileName, Path::Brotato);
			}
		}
	}
}

CPlayerCharacter* CPlayerCharacter::Clone()
{
	return new CPlayerCharacter(*this);
}
