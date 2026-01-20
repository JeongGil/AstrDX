#pragma once
#include "CInventoryItem.h"

#include <EngineInfo.h>

class CInventoryItem_Weapon :
    public CInventoryItem
{
public:
	TableID GetWeaponInfoID() const
	{
		return WeaponInfoID;
	}

	void SetWeaponInfoID(TableID ID)
	{
		WeaponInfoID = ID;
	}

private:
	TableID WeaponInfoID = TableID(-1);

	// TODO: Statistics such as other acquired stats, damage dealt in the previous round, etc.
};

