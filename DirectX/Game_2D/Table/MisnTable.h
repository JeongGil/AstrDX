#pragma once
#include <Table/TableBase.h>

#include "MiscInfo.h"

class MiscTable : public TableBase<MiscTable, FMiscInfo>
{
private:
	MiscTable() = default;

protected:
	~MiscTable() override = default;
};
