#pragma once
#include "Object/CGameObject.h"

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
	void Skill1Press();
	void Skill1Hold();
	void Skill1Release();

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CSceneComponent> Rotation;
	std::weak_ptr<CMeshComponent> SubMeshComponent;
	std::weak_ptr<CCameraComponent> CameraComponent;
	std::weak_ptr<CStateComponent> StateComponent;
	std::weak_ptr<CAnimation2DComponent> Animation2DComponent;

	bool bOnAttack = false;

public:
	bool Init() override;
	void Update(float DeltaTime) override;
	void Destroy() override;
	void Render() override
	{
		CGameObject::Render();
	}

protected:
	CPlayer() = default;
	CPlayer(const CPlayer& ref) = default;
	CPlayer(CPlayer&& ref) noexcept = default;

public:
	~CPlayer() override = default;
};

