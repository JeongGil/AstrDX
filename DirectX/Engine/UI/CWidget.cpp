#include "CWidget.h"

#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CCBufferUIDefault.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../World/CWorld.h"

bool CWidget::SortRender(const std::shared_ptr<CWidget>& A, const std::shared_ptr<CWidget>& B)
{
	return A->GetZOrder() < B->GetZOrder();
}

bool CWidget::SortCollision(const std::shared_ptr<CWidget>& A, const std::shared_ptr<CWidget>& B)
{
	return A->GetZOrder() > B->GetZOrder();
}

CWidget::CWidget()
{
}

CWidget::~CWidget()
{
}

void CWidget::SetShader(const std::weak_ptr<CShader>& Shader)
{
	this->Shader = Shader;
}

void CWidget::SetShader(const std::string& Key)
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		Shader = ShaderMgr->FindShader(Key);
	}
}

bool CWidget::Init()
{
	if (auto World = this->World.lock())
	{
		auto AssetMgr = World->GetWorldAssetManager().lock();
		if (auto Mesh = AssetMgr->FindMesh("UIRectTex").lock())
		{
			this->Mesh = Mesh;
		}
	}
	else
	{
		auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();
		Mesh = MeshMgr->FindMesh("Mesh_UIRectTex");
	}

	SetShader("UIDefault");

	TransformCBuffer.reset(new CCBufferTransform);
	TransformCBuffer->Init();

	UIDefaultCBuffer.reset(new CCBufferUIDefault);
	UIDefaultCBuffer->Init();

	return true;
}

void CWidget::Update(const float DeltaTime)
{
}

void CWidget::Render()
{
	if (auto Parent = this->Parent.lock())
	{
		RenderPos = Parent->RenderPos + Pos;
	}
	else
	{
		RenderPos = Pos;
	}

	UIDefaultCBuffer->SetWidgetColor(WidgetColor);
}

void CWidget::Destroy()
{
	bAlive = false;
}

bool CWidget::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	if (auto Parent = this->Parent.lock())
	{
		RenderPos = Parent->RenderPos + Pos;
	}
	else
	{
		RenderPos = Pos;
	}

	if (Angle == 0.f)
	{
		FVector Min = RenderPos - Size * Pivot;
		FVector Max = Min + Size;

		if (MousePos.x < Min.x)
		{
			if (bMouseOn)
			{
				bMouseOn = false;
				MouseUnHovered();
			}

			return false;
		}
		else if (MousePos.x > Max.x)
		{
			if (bMouseOn)
			{
				bMouseOn = false;
				MouseUnHovered();
			}

			return false;
		}
		else if (MousePos.y < Min.y)
		{
			if (bMouseOn)
			{
				bMouseOn = false;
				MouseUnHovered();
			}

			return false;
		}
		else if (MousePos.y > Max.y)
		{
			if (bMouseOn)
			{
				bMouseOn = false;
				MouseUnHovered();
			}

			return false;
		}

		Result = std::dynamic_pointer_cast<CWidget>(shared_from_this());
		bMouseOn = true;
		MouseHovered();

		return true;
	}

	FVector Axis[2];
	FVector HalfSize = Size * 0.5f;
	FVector Center = RenderPos - Size * Pivot + HalfSize;

	Axis[0] = FVector::Axis[EAxis::X];
	Axis[1] = FVector::Axis[EAxis::Y];

	FMatrix	RotationMat;
	RotationMat.RotationZ(Angle);

	Axis[0] = Axis[0].TransformNormal(RotationMat);
	Axis[1] = Axis[1].TransformNormal(RotationMat);

	Axis[0].Normalize();
	Axis[1].Normalize();

	FVector ConvertMousePos;
	ConvertMousePos.x = MousePos.x;
	ConvertMousePos.y = MousePos.y;

	FVector CenterLine = Center - ConvertMousePos;

	float	Dist = abs(CenterLine.Dot(Axis[0]));

	if (Dist > HalfSize.x)
	{
		if (bMouseOn)
		{
			bMouseOn = false;
			MouseUnHovered();
		}

		return false;
	}

	Dist = abs(CenterLine.Dot(Axis[1]));

	if (Dist > HalfSize.y)
	{
		if (bMouseOn)
		{
			bMouseOn = false;
			MouseUnHovered();
		}

		return false;
	}

	Result = std::dynamic_pointer_cast<CWidget>(shared_from_this());;
	bMouseOn = true;
	MouseHovered();

	return true;
}

void CWidget::MouseHovered()
{
}

void CWidget::MouseUnHovered()
{
}
