#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;
class CCameraComponent;
class CStateComponent;

class CPlayer :
	public CGameObject
{
	friend class CWorld;
	friend CObject;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CSceneComponent> Rotation;
	std::weak_ptr<CMeshComponent> SubMeshComponent;
	std::weak_ptr<CCameraComponent> CameraComponent;
	std::weak_ptr<CStateComponent> StateComponent;

public:
	bool Init() override;
	void Update(float DeltaTime) override;

protected:
	CPlayer() = default;
	CPlayer(const CPlayer& ref) = default;
	CPlayer(CPlayer&& ref) noexcept = default;

public:
	~CPlayer() override = default;
};

