#pragma once
#include "CharacterVisualInfo.h"
#include "Table/TableBase.h"

class CharacterVisualTable : public TableBase<CharacterVisualTable, FCharacterVisualInfo>
{
	friend TableBase;

private:
	CharacterVisualTable() = default;

protected:
	~CharacterVisualTable() override = default;
};
