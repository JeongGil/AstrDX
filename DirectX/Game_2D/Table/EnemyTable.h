#pragma once
#include "EnemyInfo.h"
#include "Table/TableBase.h"

class EnemyTable : public TableBase<EnemyTable, FEnemyInfo>
{
	friend TableBase;

private:
	EnemyTable() = default;

protected:
	~EnemyTable() override = default;
};
