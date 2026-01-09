#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;
class CColliderBox2D;

enum class ETeam
{
	Player,
	Enemy,
};

enum class ETargetState
{
	Normal,
	Invincible,
	UnTargetable,
};

class CCharacter :
    public CGameObject
{
	friend class CWorld;
	friend CObject;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Destroy() override;

protected:
	CCharacter* Clone() override;

public:
	ETeam GetTeam() const
	{
		return Team;
	}
	void SetTeam(ETeam InTeam)
	{
		Team = InTeam;
	}
protected:
	ETeam Team;
	ETargetState TargetState = ETargetState::Normal;

	std::weak_ptr<CColliderBox2D> Collider;
	std::weak_ptr<CMeshComponent> Mesh;

protected:
	CCharacter() = default;

public:
	~CCharacter() override = default;
};

