#pragma once
#include <Table/TableBase.h>

#include "MiscInfo.h"

class MiscTable : public TableBase<MiscTable, FMiscInfo>
{
	friend TableBase;
private:
	MiscTable() = default;
	~MiscTable() override = default;

public:
	const FMiscInfo* Get() const
	{
		assert(!Items.empty());

		return Items.begin()->second;
	}
};
