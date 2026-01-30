#pragma once
#include <Object/CGameObject.h>

class CWidgetComponent;
class CColliderLine2D;
class CColliderSphere2D;
class CColliderBox2D;
class CObjectMovementComponent;
class CBullet;
class CMeshComponent;
class CCameraComponent;
class CStateComponent;
class CAnimation2DComponent;

class CPlayer :
	public CGameObject
{
	friend class CWorld;
	friend CObject;

private:
	void TestNotify();
	void AttackNotify();
	void AttackFinish();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void AttackKey();
	void JumpKey();
	void Skill1Press();
	void Skill1Hold();
	void Skill1Release();

private:

	int HP = 10;
	int MaxHP = 10;

	std::vector<std::function<void(float, float)>> OnHPChanged;
	std::vector<std::function<void(float, float)>> OnMPChanged;

	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CSceneComponent> Rotation;
	std::weak_ptr<CMeshComponent> SubMeshComponent;
	std::weak_ptr<CCameraComponent> CameraComponent;
	std::weak_ptr<CStateComponent> StateComponent;
	std::weak_ptr<CAnimation2DComponent> Animation2DComponent;
	std::weak_ptr<CObjectMovementComponent> MovementComponent;
	
	std::weak_ptr<CColliderBox2D> Body;
	std::weak_ptr<CColliderSphere2D> Sphere2D;
	std::weak_ptr<CColliderLine2D> Line2D;

	std::weak_ptr<CWidgetComponent> HUDWidget;

	bool bAutoIdle = false;

	std::weak_ptr<CBullet> Skill1Bullet;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Destroy() override;

	float TakeDamage(float Damage) override;

protected:
	CPlayer() = default;
	CPlayer(const CPlayer& ref) = default;
	CPlayer(CPlayer&& ref) noexcept = default;

public:
	~CPlayer() override = default;
};

