#pragma once
#include <UI/CWidgetContainer.h>

class CTextBlock;
class CWorldUIManager;

class CDamageFloatingText :
	public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CDamageFloatingText() = default;

public:
	~CDamageFloatingText() override = default;

private:
	std::weak_ptr<CTextBlock> Text;

	int Damage;
	float MoveSpeed = 10.f;
	
	float RemainLifeTime = 1.f;
	FVector AnchorWorldPos;
	float ScreenOffsetX = 0.f;
	float ScreenOffsetY = 0.f;
	float FloatingOffsetY = 0.f;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

	void SetDamage(float Damage);
	void SetAnchorWorldPos(const FVector& InWorldPos, float OffsetX, float OffsetY);
};

