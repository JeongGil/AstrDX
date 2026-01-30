#pragma once
#include <Table/TableBase.h>

#include "FStringInfo.h"

class StringTable : public TableBase<StringTable, FStringInfo>
{
	friend TableBase;

private:
	StringTable() = default;
	~StringTable() override = default;
};
