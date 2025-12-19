#include "CObjectComponent.h"

bool CObjectComponent::Init()
{
	return true;
}

void CObjectComponent::Update(const float Delta)
{
}

void CObjectComponent::PostUpdate(const float Delta)
{
}

void CObjectComponent::Destroy()
{
	CComponent::Destroy();
}
