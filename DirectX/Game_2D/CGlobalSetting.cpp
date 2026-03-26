#include "CGlobalSetting.h"

#include <CCollisionInfoManager.h>
#include <EngineInfo.h>
#include <Asset/CAssetManager.h>
#include <Asset/CPathManager.h>
#include <Asset/Shader/CShaderManager.h>
#include <Render/CRenderManager.h>
#include <UI/CMouseWidget.h>

#include "Strings.h"
#include "Shader/CShaderBrotatoTile.h"
#include "Shader/CShaderPostProcessHit.h"

bool CGlobalSetting::Init()
{
	if (!CEngineSetting::Init())
	{
		return false;
	}

	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Key::Path::Asset);

	CCollisionInfoManager::GetInst()->CreateChannel("PlayerAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("MonsterAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("FindEnemy");
	CCollisionInfoManager::GetInst()->CreateChannel("MapBoundary");
	CCollisionInfoManager::GetInst()->CreateChannel("DropItem");
	CCollisionInfoManager::GetInst()->CreateChannel("PickupRange");

	CCollisionInfoManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("FindEnemy", "FindEnemy", true, ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->CreateProfile("MapBoundary", "MapBoundary", true, ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->CreateProfile("DropItem", "DropItem", true, ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->CreateProfile("PickupRange", "PickupRange", true, ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("PlayerAttack", "PlayerAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("PlayerAttack", "Player", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("PlayerAttack", "MonsterAttack", ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("MonsterAttack", "Monster", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("MonsterAttack", "MonsterAttack", ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("Player", "Player", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Monster", "Monster", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Player", "Monster", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Monster", "Player", ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("Player", "MonsterAttack", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("MonsterAttack", "Player", ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("FindEnemy", "Monster", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Monster", "FindEnemy", ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("Player", "MapBoundary", ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("MapBoundary", "Player", ECollisionInteraction::Block);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("Monster", "MapBoundary", ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("MapBoundary", "Monster", ECollisionInteraction::Block);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("PlayerAttack", "Monster", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("Monster", "PlayerAttack", ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction("PickupRange", "DropItem", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction("DropItem", "PickupRange", ECollisionInteraction::Overlap);

	if (auto MouseWidget = CRenderManager::GetInst()->SetMouseWidget<CMouseWidget>(EMouseState::Normal, "MouseNormal").lock())
	{
		MouseWidget->SetSize(32, 32);
		MouseWidget->SetTexture("BrotatoMouse", TEXT("ui/custom_cursor.png"), Key::Path::Brotato);
	}

	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		ShaderMgr->CreateShader<CShaderPostProcessHit>("Hit");
		ShaderMgr->CreateShader<CShaderBrotatoTile>("BrotatoTile");
	}

	return true;
}
