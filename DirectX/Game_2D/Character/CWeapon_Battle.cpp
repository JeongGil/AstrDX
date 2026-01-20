#include "CWeapon_Battle.h"

#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <World/CWorld.h>
#include <Asset/Material/CMaterial.h>

#include "../Strings.h"
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

	return true;
}

void CWeapon_Battle::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);
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
				const FTextureInfo* TexInfo = Texture->GetTexture();
				Mesh->SetWorldScale(TexInfo->Width, TexInfo->Height);

				if (auto Col = Collider.lock())
				{
					Col->SetBoxExtend(TexInfo->Width, TexInfo->Height);
				}
			}
		}
	}
}
