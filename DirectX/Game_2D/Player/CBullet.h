#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;

class CBullet :
	public CGameObject
{
	friend class CWorld;

private:
	float Distance = 2.f;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CBullet() = default;
	CBullet(const CBullet& other) = default;
	CBullet(CBullet&& other) noexcept = default;

public:
	~CBullet() override = default;
};

