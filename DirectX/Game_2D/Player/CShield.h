#pragma once
#include "Object/CGameObject.h"

class CRotationMovementComponent;
class CMeshComponent;

class CShield :
    public CGameObject
{
	friend CObject;
	friend class CWorld;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

protected:
	CShield* Clone() override;

protected:
	CShield() = default;

public:
	~CShield() override = default;
};

