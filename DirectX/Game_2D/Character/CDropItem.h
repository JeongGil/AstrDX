#pragma once
#include <Object\CGameObject.h>

class CPlayerCharacter;
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

	bool bIsCollecting{};

	std::weak_ptr<CPlayerCharacter> Player;

public:
	bool Init() override;
	void Update(float DeltaTime) override;

protected:
	CDropItem* Clone() override;

private:
	void OnPickedUp();

public:
	[[nodiscard]] std::weak_ptr<CPlayerCharacter> GetPlayer() const
	{
		return Player;
	}

	void SetPlayer(const std::weak_ptr<CPlayerCharacter>& Player)
	{
		this->Player = Player;
	}

	[[nodiscard]] bool IsCollecting() const
	{
		return bIsCollecting;
	}

	void SetIsCollecting(const bool bIsCollecting);

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

