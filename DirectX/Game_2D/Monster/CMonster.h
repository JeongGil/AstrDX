#pragma once
#include "Object/CGameObject.h"

class CWidgetComponent;
class CColliderLine2D;
class CColliderBox2D;
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

	int HP = 10;
	int MaxHP = 10;

	std::vector<std::function<void(float, float)>> OnHPChanged;

	float DetectRange = 600.f;

	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CAnimation2DComponent> Animation2DComponent;
	std::weak_ptr<CColliderSphere2D> Body;
	std::weak_ptr<CColliderLine2D> Line2D;

	std::weak_ptr<CWidgetComponent> HUDWidget;

public:
	bool Init() override;
	void Update(float DeltaTime) override;
	float TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator) override;

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

