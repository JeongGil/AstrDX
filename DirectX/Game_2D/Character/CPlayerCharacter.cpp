#include "CPlayerCharacter.h"

bool CPlayerCharacter::Init()
{
	if (!CCharacter::Init())
	{
		return false;
	}



	return true;
}

void CPlayerCharacter::Update(const float DeltaTime)
{
	CCharacter::Update(DeltaTime);
}

void CPlayerCharacter::PostUpdate(const float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

CPlayerCharacter* CPlayerCharacter::Clone()
{
	return new CPlayerCharacter(*this);
}
