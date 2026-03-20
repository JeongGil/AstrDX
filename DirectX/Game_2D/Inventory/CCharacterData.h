#pragma once

#include <EngineInfo.h>

#include "../Defines.h"

struct FShopGoods;
class CInventoryItem_Item;
class CInventoryItem_Weapon;

class CCharacterData
{
private:
	CCharacterData() = default;
	~CCharacterData() = default;

private:
	std::vector<std::shared_ptr<CInventoryItem_Weapon>> Weapons;
	std::vector<std::shared_ptr<CInventoryItem_Item>> Items;

	std::unordered_map<EWeaponType::Type, int> WeaponTypeCounts;

	int MaterialCount{};

	int Level{};
	int CurrentExp{};

public:
	bool Init();

	std::weak_ptr<CInventoryItem_Weapon> GetWeapon(size_t SlotIdx);
	size_t GetWeaponCount() const
	{
		return Weapons.size();
	}

	void AddWeapon(const FShopGoods& WeaponGoods);
	void AddWeapon(TableID WeaponID);
	void RemoveWeapon(size_t SlotIdx);
	void RemoveWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon);

	void Clear()
	{
		Weapons.clear();
		Items.clear();
	}

	int GetWeaponTypeCount(EWeaponType::Type WeaponType) const
	{
		auto It = WeaponTypeCounts.find(WeaponType);
		if (It == WeaponTypeCounts.end())
		{
			return 0;
		}

		return It->second;
	}

	const std::unordered_map<EWeaponType::Type, int>& GetWeaponTypeCounts() const
	{
		return WeaponTypeCounts;
	}

	[[nodiscard]] int GetMaterialCount() const
	{
		return MaterialCount;
	}

	void SetMaterialCount(const int MaterialCount)
	{
		this->MaterialCount = MaterialCount;
	}

	void AddMaterialCount(const int Added)
	{
		SetMaterialCount(MaterialCount + Added);
	}

	static int GetLevelUpEXP(int CurrentLevel)
	{
		assert(CurrentLevel >= 1);
		return (1 + CurrentLevel) * (1 + CurrentLevel);
	}

private:
	void RefreshWeaponTypeCounts();

public:
	static CCharacterData& GetInst()
	{
		static CCharacterData Inst;
		return Inst;
	}
};

