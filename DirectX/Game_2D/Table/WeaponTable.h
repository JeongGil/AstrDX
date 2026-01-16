#pragma once
#include "WeaponInfo.h"
#include "Table/TableBase.h"

class WeaponTable : public TableBase<WeaponTable, FWeaponInfo>
{
	friend class TableBase;

private:
	WeaponTable() = default;

protected:
	~WeaponTable() override = default;
};
