#pragma once
#include "ItemInfo.h"
#include "Table/TableBase.h"

class ItemTable: public TableBase<ItemTable, FItemTable>
{
private:
	ItemTable() = default;
};
