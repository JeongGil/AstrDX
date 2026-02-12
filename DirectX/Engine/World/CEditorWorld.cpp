#include "CEditorWorld.h"

CEditorWorld::CEditorWorld()
{
}

CEditorWorld::~CEditorWorld()
{
}

bool CEditorWorld::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	LoadAnimation2D();
	LoadSound();
	CreateUI();

	return false;
}

void CEditorWorld::LoadAnimation2D()
{
}

void CEditorWorld::LoadSound()
{
}

void CEditorWorld::CreateUI()
{
}
