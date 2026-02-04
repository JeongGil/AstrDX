#include "CGlobalSetting.h"

#include <CCollisionInfoManager.h>
#include <EngineInfo.h>
#include <Asset/CAssetManager.h>
#include <Asset/Shader/CShaderManager.h>
#include <Render/CRenderManager.h>
#include <UI/CMouseWidget.h>

#include "Shader/CShaderPostProcessHit.h"

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

	if (auto MouseWidget = CRenderManager::GetInst()->SetMouseWidget<CMouseWidget>(EMouseState::Normal, "MouseNormal").lock())
	{
		std::vector<const TCHAR*> TextureFileName;
		for (int i = 0; i < 12; i++)
		{
			TCHAR* FileName = new TCHAR[MAX_PATH];
			memset(FileName, 0, sizeof(TCHAR)* MAX_PATH);
			wsprintf(FileName, TEXT("Mouse/Default/%d.png"), i);

			TextureFileName.push_back(FileName);
		}

		MouseWidget->SetSize(32.f, 31.f);
		MouseWidget->SetTextures("MouseNormal", TextureFileName);

		MouseWidget->AddBrushFrame(0.f, 0.f, 32.f, 31.f, 13);
		MouseWidget->SetBrushAnimation(true);

		for (auto& Name : TextureFileName)
		{
			delete[] Name;
		}

		TextureFileName.clear();
	}

	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		ShaderMgr->CreateShader<CShaderPostProcessHit>("Hit");
	}

	return true;
}
