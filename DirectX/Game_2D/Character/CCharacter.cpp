#include "CCharacter.h"

#include "../Strings.h"

bool CCharacter::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Collider = CreateComponent<CColliderBox2D>(Comp::Collider);

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
