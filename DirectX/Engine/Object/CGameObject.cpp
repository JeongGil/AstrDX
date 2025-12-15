#include "CGameObject.h"

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	if (const auto Root = this->Root.lock())
	{
		Root->Update(DeltaTime);
	}
}

void CGameObject::Render()
{
	if (const auto Root = this->Root.lock())
	{
		Root->Render();
	}
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::Destroy()
{
	bAlive = false;
}

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}
