#pragma once
#include "ItemInfo.h"
#include "Table/TableBase.h"

class ItemTable: public TableBase<ItemTable, FItemInfo>
{
	friend TableBase;

private:
	ItemTable() = default;
	~ItemTable() override = default;
};
