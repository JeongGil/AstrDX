#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;

class CMonster :
    public CGameObject
{
	friend class CWorld;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	float FireCoolTime = 1.f;

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

