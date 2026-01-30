#include "CWidgetComponent.h"

#include "../CDevice.h"

CWidgetComponent::CWidgetComponent()
{
	RenderType = EComponentRender::Render;
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& Other) :
	CSceneComponent(Other)
{
}

CWidgetComponent::CWidgetComponent(CWidgetComponent&& Other) noexcept :
	CSceneComponent(Other)
{
}

CWidgetComponent::~CWidgetComponent()
{
}

void CWidgetComponent::Begin()
{
	CSceneComponent::Begin();
}

bool CWidgetComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}

	return true;
}

void CWidgetComponent::Update(const float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (Widget)
	{
		Widget->Update(DeltaTime);
	}
}

void CWidgetComponent::PostUpdate(const float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CWidgetComponent::Render()
{
	CSceneComponent::Render();

	if (Widget)
	{
		FVector Size = Widget->GetSize();

		FResolution RS = CDevice::GetInst()->GetResolution();

		FVector CamPos = World.lock()->GetCameraManager().lock()->GetMainCameraWorldPosition();

		FVector LT;
		LT.x = CamPos.x - RS.Width * 0.5f;
		LT.y = CamPos.y + RS.Width * 0.5f;

		FVector Pos;
		Pos.x = WorldPosition.x - LT.x;
		Pos.y = LT.y - WorldPosition.y;

		Pos.x -= Size.x * 0.5f;
		Pos.y -= Size.y * 0.5f;

		Widget->SetPos(Pos);
		
		Widget->Render();
	}
}

void CWidgetComponent::PostRender()
{
	CSceneComponent::PostRender();
}

void CWidgetComponent::Destroy()
{
	CSceneComponent::Destroy();
}

CWidgetComponent* CWidgetComponent::Clone() const
{
	return new CWidgetComponent(*this);
}
