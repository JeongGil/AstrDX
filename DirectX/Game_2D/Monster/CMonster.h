#pragma once
#include "Object/CGameObject.h"

class CColliderSphere2D;
class CAnimation2DComponent;
class CMeshComponent;

class CMonster :
    public CGameObject
{
	friend class CWorld;
	friend CObject;

private:
	float ElapsedFromShot = 0.f;
	std::weak_ptr<CGameObject> FireTarget;

	float DetectRange = 400.f;

	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CAnimation2DComponent> Animation2DComponent;
	std::weak_ptr<CColliderSphere2D> Body;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CMonster* Clone() override;

protected:
	CMonster() = default;
	CMonster(const CMonster& other) = default;
	CMonster(CMonster&& other) noexcept = default;

public:
	~CMonster() override = default;

private:
	void AttackNotify();
	void AttackFinish();

	bool bOnAttack = false;
};

