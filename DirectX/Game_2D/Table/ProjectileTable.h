#pragma once
#include "ProjectileInfo.h"
#include "Table/TableBase.h"

class ProjectileTable : public TableBase<ProjectileTable, FProjectileInfo>
{
	friend TableBase;

private:
	ProjectileTable() = default;

protected:
	~ProjectileTable() override = default;
};
