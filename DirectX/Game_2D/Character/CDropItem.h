#pragma once
#include <Object\CGameObject.h>

class CColliderBox2D;
class CMeshComponent;
class CWorld;

enum class EDropItemType
{
	Material,
	Fruit,
	ItemBox,
	LegendaryItemBox,	
};

class CDropItem :
    public CGameObject
{
	friend CWorld;
	friend CObject;

protected:
	CDropItem() = default;
	CDropItem(const CDropItem& Other) = default;
	CDropItem(CDropItem&& Other) noexcept = default;
	CDropItem& operator=(const CDropItem& Other) = default;
	CDropItem& operator=(CDropItem&& Other) noexcept = default;

public:
	~CDropItem() override = default;

private:
	EDropItemType ItemType{};
	std::weak_ptr<CMeshComponent> Mesh;
	std::weak_ptr<CColliderBox2D> Collider;

	int MaterialCount{};

public:
	bool Init() override;

protected:
	CDropItem* Clone() override;

public:
	[[nodiscard]] EDropItemType GetItemType	() const
	{
		return ItemType;
	}

	void SetItemType(const EDropItemType ItemType);

	[[nodiscard]] int GetMaterialCount() const
	{
		return MaterialCount;
	}

	void SetMaterialCount(const int MaterialCount)
	{
		this->MaterialCount = MaterialCount;
	}
};

