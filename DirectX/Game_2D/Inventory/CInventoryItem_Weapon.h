#pragma once
#include "CInventoryItem.h"

class CInventoryItem_Weapon :
    public CInventoryItem
{
	TableID GetWeaponInfoID() const
	{
		return WeaponInfoID;
	}

private:
	TableID WeaponInfoID;
};

