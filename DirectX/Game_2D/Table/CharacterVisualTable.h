#pragma once
#include "CharacterVisualInfo.h"
#include "Table/TableBase.h"

class CharacterVisualTable : public TableBase<CharacterVisualTable, FCharacterVisualInfo>
{
private:
	CharacterVisualTable() = default;

protected:
	~CharacterVisualTable() override = default;
};
