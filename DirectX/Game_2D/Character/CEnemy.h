#pragma once
#include "CCharacter.h"

class CPlayerCharacter;

class CEnemy :
    public CCharacter
{
public:
	bool Init() override;
	void Update(float DeltaTime) override;
	float TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator) override;

protected:
	CEnemy* Clone() override;

protected:
	std::weak_ptr<CMeshComponent> Mesh;

private:
	std::weak_ptr<CPlayerCharacter> Player;
};

