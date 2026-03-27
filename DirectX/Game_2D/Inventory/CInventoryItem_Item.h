#pragma once
#include "CInventoryItem.h"

#include <EngineInfo.h>

class CInventoryItem_Item :
	public CInventoryItem
{
private:
	TableID ItemInfoID{ -1 };

	int Count = 0;

public:
	[[nodiscard]] TableID GetItemInfoID() const
	{
		return ItemInfoID;
	}

	void SetItemInfoID(TableID ID)
	{
		ItemInfoID = ID;
	}

	[[nodiscard]] int GetItemCount() const
	{
		return Count;
	}

	void SetItemCount(int NewCount)
	{
		Count = NewCount;
	}
};

