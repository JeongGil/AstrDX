#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;
class CColliderBox2D;

enum class ETeam
{
	Player,
	Enemy,
	Neutral,
};

enum class ETargetState
{
	Normal,
	Invincible,
	UnTargetable,
};

enum class ESpawnState
{
	Spawning,      // 탄생 연출 중
	PendingNotify, // 연출 완료, 콜백 호출 대기
	Complete       // 모든 처리 완료
};

class CCharacter :
	public CGameObject
{
	friend class CWorld;
	friend CObject;

protected:
	CCharacter() = default;

public:
	~CCharacter() override = default;

private:
	float SSElapsed = 0.f;
	float SSIntensity = 0.05f;
	float SSPeriod = 1.f;

	bool bIsSpiralShrinking = false;
	float SpiralShrinkElapsed = 0.f;
	float SpiralShrinkDuration = 0.5f;
	float SpiralRotationSpeed = 720.f;

	bool bIsPendingDead = false;

	ESpawnState SpawnState = ESpawnState::Spawning;

protected:
	ETeam Team{};
	ETargetState TargetState = ETargetState::Normal;

	std::weak_ptr<CSceneComponent> Root;
	std::weak_ptr<CColliderBox2D> Collider;

	float TimeToSpawn{};
	float ElapsedSpawnTime{};	

	std::vector<std::weak_ptr<CComponent>> SleepOnSpawnComponents;

public:
	bool Init() override;
	void Update(float DeltaTime) override;
	void Destroy() override;

protected:
	CCharacter* Clone() override;

public:
	float TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator) override;

	ETeam GetTeam() const
	{
		return Team;
	}
	void SetTeam(ETeam InTeam)
	{
		Team = InTeam;
	}

	[[nodiscard]] bool IsPendingDead() const
	{
		return bIsPendingDead;
	}

protected:
	void SetSquashAndStretchParams(float Intensity, float Period)
	{
		SSIntensity = Intensity;
		SSPeriod = Period;
	}

	virtual void OnDead();
	void SpiralShrink(float Duration = 0.5f, float RotationSpeed = 720.f);

	virtual void UpdateSpawnSequence(const float DeltaTime);
	virtual void OnSpawnFinished();
	[[nodiscard]] ESpawnState GetSpawnState() const
	{
		return SpawnState;
	}

private:
	void SquashAndStretch(const float DeltaTime, float Intensity, float Period);
};

