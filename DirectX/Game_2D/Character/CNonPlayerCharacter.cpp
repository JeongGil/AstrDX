#include "CNonPlayerCharacter.h"

#include <Component/CMeshComponent.h>
#include <World/CWorld.h>
#include <Component/CColliderBox2D.h>

#include "CPlayerCharacter.h"
#include "../Strings.h"
#include "../Table/MiscInfo.h"
#include "../Table/MiscTable.h"

bool CNonPlayerCharacter::Init()
{
	if (!CCharacter::Init())
	{
		return false;
	}

	if (auto Col = Collider.lock())
	{
		Col->SetCollisionProfile("Monster");
	}

	SetTeam(ETeam::Enemy);

	Mesh = CreateComponent<CMeshComponent>(Key::Comp::Mesh);
	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(150, 150);
		if (auto Col = Collider.lock())
		{
			Col->SetBoxExtent(Mesh->GetWorldScale().x, Mesh->GetWorldPosition().y);
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

void CNonPlayerCharacter::Update(const float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	if (auto World = this->World.lock())
	{
		Player = World->FindObjectOfType<CPlayerCharacter>();
		if (auto Player = this->Player.lock())
		{
			auto Dir = (Player->GetWorldPosition() - GetWorldPosition()).GetNormalized();

			constexpr float SPEED = 100.f;
			auto DelMove = Dir * SPEED * DeltaTime;
			AddWorldPosition(DelMove);
		}
	}
}

CNonPlayerCharacter* CNonPlayerCharacter::Clone()
{
	return new CNonPlayerCharacter(*this);
}
