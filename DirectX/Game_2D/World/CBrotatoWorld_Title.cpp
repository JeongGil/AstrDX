#include "CBrotatoWorld_Title.h"

#include <Asset/CPathManager.h>

#include "../Strings.h"
#include "../UI/CTitleWidget.h"

bool CBrotatoWorld_Title::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Key::Path::Asset);

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
