#include "CCollider.h"

#include "../CCollisionInfoManager.h"
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

void CCollider::SetCollisionProfile(const std::string& Key)
{
	Profile = CCollisionInfoManager::GetInst()->FindProfile(Key);
}

bool CCollider::CheckCollidingObject(CCollider* Other) const
{
	return CollidingObjects.contains(Other);
}

void CCollider::EraseCollidingObject(CCollider* Other)
{
	auto It = CollidingObjects.find(Other);
	if (It != CollidingObjects.end())
	{
		CollidingObjects.erase(It);
	}
}

void CCollider::CallOnCollisionBegin(const FVector& HitPoint, const std::weak_ptr<CCollider>& Other)
{
	if (auto Dest = Other.lock())
	{
		CollidingObjects.emplace(Dest.get(), Other);

		if (OnCollisionBegin)
		{
			OnCollisionBegin(HitPoint, Dest.get());
		}
	}
}

void CCollider::CallOnCollisionEnd(CCollider* Other)
{
	CollidingObjects.erase(Other);

	if (OnCollisionEnd)
	{
		OnCollisionEnd(Other);
	}
}

void CCollider::CallOnCollisionBlock(const FVector& HitPoint, const std::weak_ptr<CCollider>& Other)
{
	if (auto _Other = Other.lock())
	{
		if (OnCollisionBlock)
		{
			OnCollisionBlock(HitPoint, _Other.get());
		}
	}
}

void CCollider::OnCollisionBlockEnd()
{
}

void CCollider::CallOnCollisionMouseBegin(const FVector& MousePos)
{
	if (OnCollisionMouseBegin)
	{
		OnCollisionMouseBegin(MousePos);
	}
}

void CCollider::CallOnCollisionMouseEnd(const FVector& MousePos)
{
	if (OnCollisionMouseEnd)
	{
		OnCollisionMouseEnd(MousePos);
	}
}

bool CCollider::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}

	TrySetRenderLayer(ERenderOrder::DebugDraw);

	if (bDrawDebug)
	{
		TransformCBuffer.reset(new CCBufferTransform);
		TransformCBuffer->Init();
	}

	Profile = CCollisionInfoManager::GetInst()->FindProfile("Static");

	if (auto World = this->World.lock())
	{
		if (auto ColMgr = World->GetCollision().lock())
		{
			ColMgr->AddCollider(std::dynamic_pointer_cast<CCollider>(shared_from_this()));
		}
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
				ProjMat = CamMgr->GetProjMatrix();
			}
		}

		auto Mesh = this->Mesh.lock();

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
		TransformCBuffer->SetProjMatrix(ProjMat);

		FVector PivotSize = Pivot * Mesh->GetMeshSize();
		TransformCBuffer->SetPivotSize(PivotSize);

		TransformCBuffer->UpdateBuffer();

		if (IsColliding())
		{
			ColliderCBuffer->SetColor(FColor::Red);
		}
		else
		{
			ColliderCBuffer->SetColor(FColor::Green);
		}

		ColliderCBuffer->UpdateBuffer();

		if (auto Shader = this->Shader.lock())
		{
			Shader->SetShader();
		}

		Mesh->Render();
	}
}

CCollider::CCollider(CCollider&& other) noexcept :
	CSceneComponent(std::move(other)),
	ColliderType(other.ColliderType),
	Min(std::move(other.Min)),
	Max(std::move(other.Max)),
	RenderScale(std::move(other.RenderScale)),
	bDrawDebug(other.bDrawDebug),
	Profile(other.Profile),
	CollidingObjects(std::move(other.CollidingObjects)),
	Shader(std::move(other.Shader)),
	Mesh(std::move(other.Mesh)),
	TransformCBuffer(std::move(other.TransformCBuffer)),
	ColliderCBuffer(std::move(other.ColliderCBuffer)),
	OnCollisionBegin(std::move(other.OnCollisionBegin)),
	OnCollisionEnd(std::move(other.OnCollisionEnd)),
	OnCollisionBlock(std::move(other.OnCollisionBlock)),
	OnCollisionMouseBegin(std::move(other.OnCollisionMouseBegin)),
	OnCollisionMouseEnd(std::move(other.OnCollisionMouseEnd))
{
	other.Profile = nullptr;
}

CCollider::~CCollider()
{
	for (const auto& WeakOther : CollidingObjects | std::views::values)
	{
		if (auto Other = WeakOther.lock())
		{
			Other->EraseCollidingObject(this);
		}
	}
}
