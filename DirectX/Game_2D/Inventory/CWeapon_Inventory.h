#pragma once
#include "../Table/WeaponInfo.h"

class CWeapon_Inventory
{
public:
	const FWeaponInfo& GetTableData() const
	{
		return TableData;
	}

	void SetTableData(const FWeaponInfo& TableData)
	{
		this->TableData = TableData;
	}
private:
	FWeaponInfo TableData;
};

