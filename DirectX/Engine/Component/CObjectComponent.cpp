#include "CObjectComponent.h"

void CObjectComponent::Begin()
{
	CComponent::Begin();
}

bool CObjectComponent::Init()
{
	if (!CComponent::Init())
	{
		return false;
	}

	return true;
}

void CObjectComponent::Update(const float DeltaTime)
{
}

void CObjectComponent::PostUpdate(const float DeltaTime)
{
}

void CObjectComponent::Destroy()
{
	CComponent::Destroy();
}
