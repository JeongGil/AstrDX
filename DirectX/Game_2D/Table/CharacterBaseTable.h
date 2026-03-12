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
};
