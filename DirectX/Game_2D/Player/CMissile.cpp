#include "CMissile.h"

#include <Component/CMeshComponent.h>
#include <World/CWorld.h>

#include "CDevice.h"
#include "Component/CCameraComponent.h"

void CMissile::SetCamera()
{
	if (auto World = this->World.lock())
	{
		if (auto CamMgr = World->GetCameraManager().lock())
		{
			CamMgr->SetMainCamera(CameraComponent);
		}
	}
}

bool CMissile::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("Color2D");
		Mesh->SetMesh("CenterRectColor");
		Mesh->SetRelativeScale(50, 100);
	}

	CameraComponent = CreateComponent<CCameraComponent>("MissileCamera");
	if (auto Cam = CameraComponent.lock())
	{
		const auto& Resolution = CDevice::GetInst()->GetResolution();
		Cam->SetProjection(CCameraComponent::EProjectionType::Orthogonal,
			90.f, static_cast<float>(Resolution.Width), static_cast<float>(Resolution.Height), 1000.f);

		Cam->SetInheritRotation(false);
	}

	return true;
}

void CMissile::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float MoveLength = 300 * DeltaTime;
	FVector Move = GetAxis(EAxis::Y) * MoveLength;

	Distance -= MoveLength;

	AddWorldPosition(Move);

	if (Distance <= 0)
	{
		Destroy();
	}
}
