#include "CGameObject.h"

void CGameObject::SetWorld(const std::weak_ptr<CWorld>& World)
{
	this->World = World;

	for (const auto& Cmp : SceneComponents)
	{
		Cmp->SetWorld(World);
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(const float DeltaTime)
{
	if (const auto Root = this->Root.lock())
	{
		Root->Update(DeltaTime);
	}
}

void CGameObject::PostUpdate(const float DeltaTime)
{
	if (const auto Root = this->Root.lock())
	{
		Root->PostUpdate(DeltaTime);
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
