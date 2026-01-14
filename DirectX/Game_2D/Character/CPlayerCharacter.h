#pragma once
#include "CCharacter.h"

#include "../Table/CharacterVisualInfo.h"

struct FCharacterVisualInfo;

class CPlayerCharacter :
    public CCharacter
{
public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

	void SetCharacterVisual(const FCharacterVisualInfo& VisualInfo);

protected:
	CPlayerCharacter* Clone() override;

protected:
	FCharacterVisualInfo CharacterVisualInfo;

	std::weak_ptr<CMeshComponent> Potato;
	std::weak_ptr<CMeshComponent> Leg;
	std::vector<std::weak_ptr<CMeshComponent>> Decos;

protected:
	CPlayerCharacter() = default;
	CPlayerCharacter(const CPlayerCharacter& other) = default;
	CPlayerCharacter(CPlayerCharacter&& other) noexcept = default;
	CPlayerCharacter& operator=(const CPlayerCharacter& other) = default;
	CPlayerCharacter& operator=(CPlayerCharacter&& other) noexcept = default;

public:
	~CPlayerCharacter() override = default;
};

