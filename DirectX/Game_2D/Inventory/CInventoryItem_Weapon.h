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

private:
	TableID WeaponInfoID;
};

