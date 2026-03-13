#pragma once
#include "CharacterBaseInfo.h"
#include "Table/TableBase.h"

class CharacterBaseTable : public TableBase<CharacterBaseTable, FCharacterBaseInfo>
{
	friend TableBase;

private:
	CharacterBaseTable() = default;

protected:
	~CharacterBaseTable() override = default;

public:
	const FCharacterBaseInfo* Get() const
	{
		assert(!Items.empty());

		return Items.begin()->second;
	}
};
