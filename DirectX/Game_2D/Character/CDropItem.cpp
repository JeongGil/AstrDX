#include "CDropItem.h"

#include <CEngine.h>
#include <Asset/Material/CMaterial.h>
#include <Asset/Texture/CTexture.h>
#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>

#include "../Strings.h"
#include "../Table/MiscInfo.h"
#include "../Table/MiscTable.h"
#include "../Utility.h"

bool CDropItem::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Mesh = CreateComponent<CMeshComponent>(Key::Comp::Mesh);
	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->SetShader("MaterialColor2D");
		Mesh->SetMesh("CenterRectColor");

		Mesh->SetInheritScale(false);

		Mesh->SetRenderLayer(ERenderOrder::Enemy);
	}

	Collider = CreateComponent<CColliderBox2D>(Key::Comp::Collider);
	if (auto Col = Collider.lock())
	{
		Col->SetCollisionProfile("DropItem");

		Col->SetDrawDebug(true);
		Col->SetInheritScale(false);
	}

	return true;
}

CDropItem* CDropItem::Clone()
{
	return new CDropItem(*this);
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
		SetMeshAndColliderSizeFromTexture(MatTexInfo, this->Mesh, Collider);
	}
}
