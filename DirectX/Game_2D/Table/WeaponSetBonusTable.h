#pragma once
#include <Table/TableBase.h>

#include "WeaponSetBonusInfo.h"

class WeaponSetBonusTable : public TableBase<WeaponSetBonusTable, FWeaponSetBonusInfo>
{
	friend TableBase;

private:
	WeaponSetBonusTable() = default;
	~WeaponSetBonusTable() override = default;

public:
	static void GetWeaponSetBonusStats(EWeaponType::Type WeaponType, int Count, std::map<EStat::Type, int>& OutStats)
	{
		OutStats.clear();

		if (Count < 2 || Count > INVENTORY_MAX_WEAPON)
		{
			return;
		}

		TableID ID(static_cast<int>(WeaponType));
		FWeaponSetBonusInfo* Info;
		if (WeaponSetBonusTable::GetInst().TryGet(ID, Info))
		{
			FBonusStat Bonus = Info->Bonus1[Count - 1];
			EStat::Type StatType = Bonus.StatType;
			OutStats[StatType] = Bonus.StatValue;

			Bonus = Info->Bonus2[Count - 1];
			StatType = Bonus.StatType;
			OutStats[StatType] = Bonus.StatValue;
		}
	}
};
