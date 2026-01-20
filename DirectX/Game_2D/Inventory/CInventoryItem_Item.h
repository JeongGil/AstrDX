#pragma once
#include "CInventoryItem.h"

#include <EngineInfo.h>

class CInventoryItem_Item :
    public CInventoryItem
{
public:
	TableID GetItemInfoID()const
	{
		return ItemInfoID;
	}

private:
	TableID ItemInfoID;
};

