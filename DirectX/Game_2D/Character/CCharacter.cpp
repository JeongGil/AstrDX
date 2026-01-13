#include "CCharacter.h"

#include "Component/CColliderBox2D.h"

#include "../Strings.h"

bool CCharacter::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Root = CreateComponent<CSceneComponent>(Comp::Root);

	Collider = CreateComponent<CColliderBox2D>(Comp::Collider);
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetCollisionProfile("Character");
		Collider->SetDrawDebug(false);
		Collider->SetInheritScale(false);
	}

	return true;
}

void CCharacter::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCharacter::Destroy()
{
	CGameObject::Destroy();
}

CCharacter* CCharacter::Clone()
{
	return new CCharacter(*this);
}
