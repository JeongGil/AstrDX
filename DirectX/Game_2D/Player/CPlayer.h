#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;

class CPlayer :
	public CGameObject
{
	friend class CWorld;

public:
	bool Init() override;
	void Update(float DeltaTime) override;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CSceneComponent> Rotation;
	std::weak_ptr<CMeshComponent> SubMeshComponent;

public:
	CPlayer() = default;
	CPlayer(const CPlayer& ref) = default;
	CPlayer(CPlayer&& ref) noexcept = default;
	~CPlayer() override = default;
};

