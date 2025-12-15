#include "CGameObject.h"

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float deltaTime)
{
}

void CGameObject::Render()
{
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
