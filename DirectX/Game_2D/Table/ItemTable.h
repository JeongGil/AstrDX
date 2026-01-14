#pragma once
#include "ItemInfo.h"
#include "Table/TableBase.h"

class ItemTable: public TableBase<ItemTable, FItemInfo>
{
private:
	ItemTable() = default;

protected:
	~ItemTable() override = default;
};
