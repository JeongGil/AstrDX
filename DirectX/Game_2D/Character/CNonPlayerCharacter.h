#pragma once
#include "CCharacter.h"

class CPlayerCharacter;

class CNonPlayerCharacter :
    public CCharacter
{
public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CNonPlayerCharacter* Clone() override;

protected:
	std::weak_ptr<CMeshComponent> Mesh;

private:
	std::weak_ptr<CPlayerCharacter> Player;
};

