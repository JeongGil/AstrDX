#include "CObjectComponent.h"

bool CObjectComponent::Init()
{
	return CComponent::Init();
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
