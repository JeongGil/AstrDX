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

public:
	CPlayer() = default;

	CPlayer(const CPlayer& ref)
		: CGameObject(ref)
	{
	}

	CPlayer(CPlayer&& ref) noexcept
		: CGameObject(std::move(ref))
	{
	}

	~CPlayer() override = default;
};

