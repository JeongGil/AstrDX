#include "CStateComponent.h"

#include <Object/CGameObject.h>

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
		if (auto Owner = this->Owner.lock())
		{
			Owner->Destroy();
		}

		return false;
	}

	return true;
}

CComponent* CStateComponent::Clone() const
{
	return new CStateComponent(*this);
}
