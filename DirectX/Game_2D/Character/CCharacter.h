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
	void Update(float DeltaTime) override;
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
	void SetSquashAndStretchParams(float Intensity, float Period)
	{
		SSIntensity = Intensity;
		SSPeriod = Period;
	}

private:
	void SquashAndStretch(const float DeltaTime, float Intensity, float Period);
	float SSElapsed = 0.f;
	float SSIntensity = 0.05f;
	float SSPeriod = 1.f;

protected:
	ETeam Team{};
	ETargetState TargetState = ETargetState::Normal;

	std::weak_ptr<CSceneComponent> Root;
	std::weak_ptr<CColliderBox2D> Collider;

protected:
	CCharacter() = default;

public:
	~CCharacter() override = default;
};

