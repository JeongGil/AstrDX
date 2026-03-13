#include "CCameraObject.h"

#include <CDevice.h>
#include <Component/CCameraComponent.h>
#include <World/CWorld.h>

bool CCameraObject::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	CameraComponent = CreateComponent<CCameraComponent>("Cam");
	if (auto Cam = CameraComponent.lock())
	{
		const auto& Resolution = CDevice::GetInst()->GetResolution();
		Cam->SetProjection(ECameraProjectionType::Orthogonal,
			90.f, static_cast<float>(Resolution.Width), static_cast<float>(Resolution.Height), 1000);

		Cam->SetInheritRotation(false);
	}

	return true;
}

void CCameraObject::PostUpdate(const float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	if (auto Anchor = this->Anchor.lock())
	{
		SetWorldPosition(Anchor->GetWorldPosition());
	}
}

CCameraObject* CCameraObject::Clone()
{
	return new CCameraObject(*this);
}
