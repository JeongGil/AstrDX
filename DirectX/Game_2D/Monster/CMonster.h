#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;

class CMonster :
    public CGameObject
{
	friend class CWorld;

private:
	float ElapsedFromShot = 0.f;
	std::weak_ptr<CGameObject> FireTarget;

	float DetectRange = 400.f;

	std::weak_ptr<CMeshComponent> MeshComponent;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CMonster() = default;
	CMonster(const CMonster& other) = default;
	CMonster(CMonster&& other) noexcept = default;

public:
	~CMonster() override = default;
};

