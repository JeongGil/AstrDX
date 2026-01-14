#pragma once
#include "WeaponInfo.h"
#include "Table/TableBase.h"

class WeaponTable : public TableBase<WeaponTable, FWeaponInfo>
{
private:
	WeaponTable() = default;

protected:
	~WeaponTable() override = default;
};
