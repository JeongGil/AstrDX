#include "CDropItem.h"

#include <CEngine.h>
#include <Asset/Material/CMaterial.h>
#include <Asset/Texture/CTexture.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>

#include "CPlayerCharacter.h"
#include "../Strings.h"
#include "../Table/MiscInfo.h"
#include "../Table/MiscTable.h"
#include "../Utility.h"
#include "../Inventory/CCharacterData.h"

bool CDropItem::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Mesh = CreateComponent<CMeshComponent>(Key::Comp::Mesh);
	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");

		Mesh->SetInheritScale(false);

		Mesh->SetBlendState(0, "AlphaBlend");

		Mesh->SetRenderLayer(ERenderOrder::CharacterBody);
	}

	Collider = CreateComponent<CColliderBox2D>(Key::Comp::Collider);
	if (auto Col = Collider.lock())
	{
		Col->SetCollisionProfile("DropItem");

		//Col->SetDrawDebug(true);
		Col->SetInheritScale(false);

		Col->SetBoxHalfExtent(1, 1);
	}

	return true;
}

void CDropItem::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (!bIsCollecting)
	{
		return;
	}

	auto PC = Player.lock();
	if (!PC)
	{
		return;
	}

	auto Misc = MiscTable::GetInst().Get();
	auto MoveSize = DeltaTime * Misc->PickupMoveSpeed;

	auto Diff = PC->GetWorldPosition() - GetWorldPosition();
	if (Diff.SqrLength() <= MoveSize * MoveSize)
	{
		OnPickedUp();

		return;
	}

	auto Dir = Diff.GetNormalized();
	AddWorldPosition(Dir * MoveSize);
}

CDropItem* CDropItem::Clone()
{
	return new CDropItem(*this);
}

void CDropItem::OnPickedUp()
{
	CCharacterData::GetInst().AddMaterialCount(MaterialCount);
	CCharacterData::GetInst().AddExp(MaterialCount);

	Destroy();
}

void CDropItem::SetIsCollecting(const bool bIsCollecting)
{
	this->bIsCollecting = bIsCollecting;

	if (bIsCollecting)
	{
		if (auto Col = Collider.lock())
		{
			Col->SetEnable(false);
		}
	}
}

void CDropItem::SetItemType(const EDropItemType ItemType)
{
	this->ItemType = ItemType;

	auto MiscInfo = MiscTable::GetInst().Get();
	std::string TexPath;
	switch (ItemType)
	{
		case EDropItemType::Material:
		{
			std::uniform_int_distribution<int> Dist(0, MiscInfo->MaterialTexPaths.size() - 1);
			int Idx = Dist(CEngine::GetInst()->GetMT());
			TexPath = MiscInfo->MaterialTexPaths[Idx];

			if (auto Mesh = this->Mesh.lock())
			{
				Mesh->SetMaterialBaseColor(0, 0.5f, 1.0f, 0.5f, 1.0f);
			}

			break;
		}
		case EDropItemType::Fruit:
		{
			TexPath = MiscInfo->FruitTexPath;
			break;
		}
		case EDropItemType::ItemBox:
		{
			TexPath = MiscInfo->ItemBoxTexPath;
			break;
		}
		case EDropItemType::LegendaryItemBox:
		{
			TexPath = MiscInfo->LegendaryItemBoxTexPath;
			break;
		}
	}

	if (auto Mesh = this->Mesh.lock())
	{
		CA2T FileName(TexPath.c_str());
		auto MatTexInfo = Mesh->AddTexture(0, TexPath, FileName, Key::Path::Brotato);
		SetMeshSizeFromTexture(MatTexInfo, this->Mesh);
	}
}
