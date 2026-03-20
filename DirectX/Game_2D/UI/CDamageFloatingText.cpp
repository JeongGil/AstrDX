#include "CDamageFloatingText.h"

#include <UI/CTextBlock.h>
#include <CDevice.h>
#include <World/CWorld.h>

void CDamageFloatingText::SetAnchorWorldPos(const FVector& InWorldPos, float OffsetX, float OffsetY)
{
	AnchorWorldPos = InWorldPos;
	ScreenOffsetX = OffsetX;
	ScreenOffsetY = OffsetY;
	FloatingOffsetY = 0.f;
}

bool CDamageFloatingText::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	SetSize(300.f, 100.f);
	SetPivot(0.5f, 0.5f);
	SetEnable(true);

	Text = CreateWidget<CTextBlock>("Text", 1);
	if (auto Text = this->Text.lock())
	{
		Text->SetTextColor(FColor::White);
		Text->SetSize(300.f, 100.f);
		Text->SetFontSize(30.f);
		Text->SetPos(0.f, 0.f);
		Text->SetPivot(0.5f, 0.5f);
		Text->SetAlignH(ETextAlignH::Center);
		Text->SetAlignV(ETextAlignV::Middle);
		Text->SetEnable(true);
	}

	return true;
}

void CDamageFloatingText::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);

	RemainLifeTime -= DeltaTime;
	if (RemainLifeTime <= 0.f)
	{
		Destroy();
		return;
	}

	FloatingOffsetY -= MoveSpeed * DeltaTime;

	auto World = this->World.lock();
	if (!World)
	{
		const FVector DelMove = { 0, -MoveSpeed * DeltaTime, 0 };
		SetPos(GetPos() + DelMove);
		return;
	}

	auto CameraManager = World->GetCameraManager().lock();
	if (!CameraManager)
	{
		const FVector DelMove = { 0, -MoveSpeed * DeltaTime, 0 };
		SetPos(GetPos() + DelMove);
		return;
	}

	const FResolution& Resolution = CDevice::GetInst()->GetResolution();
	const FVector& CameraWorldPos = CameraManager->GetMainCameraWorldPosition();

	FVector AnchorScreenPos;
	AnchorScreenPos.x = AnchorWorldPos.x - CameraWorldPos.x + Resolution.Width * 0.5f;
	AnchorScreenPos.y = Resolution.Height - (AnchorWorldPos.y - CameraWorldPos.y + Resolution.Height * 0.5f);

	const FVector Size = GetSize();
	const float PosX = AnchorScreenPos.x + ScreenOffsetX - Size.x * 0.5f;
	const float PosY = AnchorScreenPos.y + ScreenOffsetY + FloatingOffsetY - Size.y * 0.5f;
	SetPos(PosX, PosY);
}

void CDamageFloatingText::SetDamage(float Damage)
{
	this->Damage = static_cast<int>(std::round(Damage));

	if (auto Text = this->Text.lock())
	{
		Text->SetText(this->Damage);
	}
}
