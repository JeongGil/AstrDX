#include "CGlobalSetting.h"

#include <CCollisionInfoManager.h>

bool CGlobalSetting::Init()
{
	if (!CEngineSetting::Init())
	{
		return false;
	}

	CCollisionInfoManager::GetInst()->CreateChannel("PlayerAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("MonsterAttack");

	CCollisionInfoManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("PlayerAttack", "PlayerAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("PlayerAttack", "Player", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("PlayerAttack", "MonsterAttack", ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("MonsterAttack", "Monster", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("MonsterAttack", "MonsterAttack", ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("Player", "Player", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Monster", "Monster", ECollisionInteraction::Ignore);

	return true;
}
