#include "CComponent.h"

void CComponent::Begin()
{
}

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

CComponent::CComponent(const CComponent& other): CObject(other),
                                                 //World(other.World),
                                                 //Owner(other.Owner),
                                                 //Key(other.Key),
                                                 bAlive(other.bAlive),
                                                 bEnable(other.bEnable),
                                                 Type(other.Type)
{
	SetKey(other.Key);
}

CComponent::CComponent(CComponent&& other) noexcept: CObject(std::move(other)),
                                                     //World(std::move(other.World)),
                                                     //Owner(std::move(other.Owner)),
                                                     //Key(std::move(other.Key)),
                                                     bEnable(other.bEnable),
                                                     bAlive(other.bAlive),
                                                     Type(other.Type)
{
	SetKey(std::move(other.Key));
}

CComponent::~CComponent()
{
}
