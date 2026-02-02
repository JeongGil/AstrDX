#include "CStartWorld.h"

#include <World/CWorldUIManager.h>

#include "../UI/CStartWidget.h"

CStartWorld::CStartWorld()
{
}

CStartWorld::~CStartWorld()
{
}

bool CStartWorld::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	LoadAnimation2D();

	LoadSound();

	CreateUI();

	return true;
}

void CStartWorld::LoadAnimation2D()
{
}

void CStartWorld::LoadSound()
{
	WorldAssetManager->LoadSound("MainBGM", "BGM", true, "MainBgm.mp3");
}

void CStartWorld::CreateUI()
{
	auto MainWidget = UIManager->CreateWidget<CStartWidget>("StartWidget");
}
