#pragma once
#include <Object/CGameObject.h>

class CColliderBox2D;

class CWeapon :
    public CGameObject
{
protected:

	std::weak_ptr<CColliderBox2D> Collider;
};

