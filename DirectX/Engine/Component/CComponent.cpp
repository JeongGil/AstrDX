#include "CComponent.h"

bool CComponent::Init()
{
	return true;
}

void CComponent::Update(const float Delta)
{
}

void CComponent::PostUpdate(const float Delta)
{
}

void CComponent::Render()
{
}

void CComponent::Destroy()
{
	bAlive = false;
}

CComponent::~CComponent()
{
}
