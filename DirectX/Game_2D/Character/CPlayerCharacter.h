#pragma once
#include "CCharacter.h"

class CPlayerCharacter :
    public CCharacter
{
public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

protected:
	CPlayerCharacter* Clone() override;

protected:
	std::weak_ptr<CMeshComponent> EyesMesh;
	std::weak_ptr<CMeshComponent> MouthMesh;
	std::weak_ptr<CMeshComponent> DecoMesh;
	std::weak_ptr<CMeshComponent> LegMesh;

protected:
	CPlayerCharacter() = default;
	CPlayerCharacter(const CPlayerCharacter& other) = default;
	CPlayerCharacter(CPlayerCharacter&& other) noexcept = default;
	CPlayerCharacter& operator=(const CPlayerCharacter& other) = default;
	CPlayerCharacter& operator=(CPlayerCharacter&& other) noexcept = default;

public:
	~CPlayerCharacter() override = default;
};

