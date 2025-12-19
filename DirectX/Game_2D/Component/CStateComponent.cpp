#include "CStateComponent.h"

bool CStateComponent::AddHP(int Added)
{
	HP += Added;

#ifdef _DEBUG
	char	Test[256] = {};
	sprintf_s(Test, "HP : %d\n", HP);
	OutputDebugStringA(Test);
#endif

	if (HP > MaxHP)
	{
		HP = MaxHP;
	}
	else if (HP < 0)
	{
		HP = 0;
		return false;
	}

	return true;
}

CComponent* CStateComponent::Clone() const
{
	return new CStateComponent(*this);
}
