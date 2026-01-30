#pragma once
#include <Table/TableBase.h>
#include "ItemInfo.h"

class ItemTable : public TableBase<ItemTable, FItemInfo>
{
	friend TableBase;

private:
	ItemTable() = default;
	~ItemTable() override = default;
};
