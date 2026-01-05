#include "CCollider.h"

#include "../Asset/Shader/CCBufferCollider.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CShader.h"
#include "../Render/CRenderManager.h"
#include "../World/CWorld.h"

void CCollider::SetDrawDebug(bool bDrawDebug)
{
	this->bDrawDebug = bDrawDebug;

	if (bDrawDebug)
	{
		RenderType = EComponentRender::Render;

		CRenderManager::GetInst()->AddRenderLayer(std::dynamic_pointer_cast<CSceneComponent>(shared_from_this()));

		TransformCBuffer.reset(new CCBufferTransform);
		TransformCBuffer->Init();
	}
}

bool CCollider::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}

	if (bDrawDebug)
	{
		TransformCBuffer.reset(new CCBufferTransform);
		TransformCBuffer->Init();
	}

	return true;
}

void CCollider::Update(const float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CCollider::PostUpdate(const float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CCollider::Render()
{
	CSceneComponent::Render();

	if (bDrawDebug)
	{
		FMatrix ViewMat;
		FMatrix ProjMat;

		if (auto World = this->World.lock())
		{
			if (auto CamMgr = World->GetCameraManager().lock())
			{
				ViewMat = CamMgr->GetViewMatrix();
				ProjMat = CamMgr->GetProjectionMatrix();
			}
		}

		FMatrix ScaleMat;
		FMatrix RotMat;
		FMatrix TransMat;
		FMatrix WorldMat;

		ScaleMat.Scaling(RenderScale);
		RotMat.Rotation(WorldRotation);
		TransMat.Translation(WorldPosition);

		WorldMat = ScaleMat * RotMat * TransMat;

		TransformCBuffer->SetWorldMatrix(WorldMat);
		TransformCBuffer->SetViewMatrix(ViewMat);
		TransformCBuffer->SetProjectionMatrix(ProjMat);

		TransformCBuffer->UpdateBuffer();

		ColliderCBuffer->SetColor(bCollide ? FColor::Red : FColor::Green);

		ColliderCBuffer->UpdateBuffer();

		if (auto Shader = this->Shader.lock())
		{
			Shader->SetShader();
		}

		if (auto Mesh = this->Mesh.lock())
		{
			Mesh->Render();
		}
	}
}
