#include "CBrotatoWorld_Title.h"

#include <Asset/CPathManager.h>
#include <Render/CRenderManager.h>

#include "../Strings.h"
#include "../Table/CTableManager.h"
#include "../UI/CTitleWidget.h"

bool CBrotatoWorld_Title::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	CRenderManager::GetInst()->CreateLayer("CharacterLeg", ERenderOrder::CharacterLeg, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterBody", ERenderOrder::CharacterBody, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterDeco", ERenderOrder::CharacterDeco, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterItem", ERenderOrder::CharacterItem, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterWeapon", ERenderOrder::CharacterWeapon, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("Enemy", ERenderOrder::Enemy, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("Effect", ERenderOrder::Effect, ERenderListSort::Y);

	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Key::Path::Asset);

	CTableManager::GetInst().Init();
	CTableManager::GetInst().LoadTables();

	LoadAnimation2D();
	LoadSound();
	CreateUI();

	return true;
}

void CBrotatoWorld_Title::LoadAnimation2D()
{
}

void CBrotatoWorld_Title::LoadSound()
{
}

void CBrotatoWorld_Title::CreateUI()
{
	auto MainWidget = UIManager->CreateWidget<CTitleWidget>("TitleWidget");
}
