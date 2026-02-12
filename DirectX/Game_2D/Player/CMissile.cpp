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

	MeshComponent = CreateComponent<CMeshComponent>("MissileMesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("MaterialColor2D");
		Mesh->SetMesh("CenterRectColor");
		Mesh->SetRelativeScale(50, 100);
	}

	CameraComponent = CreateComponent<CCameraComponent>("MissileCamera");
	if (auto Cam = CameraComponent.lock())
	{
		const auto& Resolution = CDevice::GetInst()->GetResolution();
		Cam->SetProjection(ECameraProjectionType::Orthogonal,
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

CMissile* CMissile::Clone()
{
	return new CMissile(*this);
}

CMissile::CMissile(const CMissile& other): CGameObject(other),
                                           MeshComponent(other.MeshComponent),
                                           CameraComponent(other.CameraComponent),
                                           Distance(other.Distance)
{
}

CMissile::CMissile(CMissile&& other) noexcept: CGameObject(std::move(other)),
                                               MeshComponent(std::move(other.MeshComponent)),
                                               CameraComponent(std::move(other.CameraComponent)),
                                               Distance(other.Distance)
{
}
