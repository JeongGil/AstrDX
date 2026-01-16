#pragma once
#include <Table/TableBase.h>

#include "MiscInfo.h"

class MiscTable : public TableBase<MiscTable, FMiscInfo>
{
	friend class TableBase;
private:
	MiscTable() = default;
	~MiscTable() override = default;
};
