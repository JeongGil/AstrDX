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

CWidget::CWidget(const CWidget& other)
{
	bEnable = other.bEnable;
	bAlive = other.bAlive;
	World = other.World;
	UIManager = other.UIManager;
	Shader = other.Shader;
	Mesh = other.Mesh;

	TransformCBuffer.reset(new CCBufferTransform);
	TransformCBuffer->Init();

	UIDefaultCBuffer.reset(new CCBufferUIDefault);
	UIDefaultCBuffer->Init();

	Key = other.Key;
	Pos = other.Pos;
	RenderPos = other.RenderPos;
	Size = other.Size;
	Pivot = other.Pivot;
	Angle = other.Angle;
	ZOrder = other.ZOrder;
	PrevZOrder = other.PrevZOrder;
	WidgetColor = other.WidgetColor;
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

bool CWidget::MouseDragStart(const FVector2& MousePos, std::shared_ptr<CWidget>& DragOperator)
{
	return false;
}

bool CWidget::MouseDrag(const FVector2& MousePos, const FVector2& MouseMove)
{
	return false;
}

bool CWidget::MouseDragEnd(const FVector2& MousePos)
{
	return false;
}

void CWidget::RenderBrush(const FUIBrush& Brush, const FVector& RenderPos, const FVector& Size)
{
	auto Shader = this->Shader.lock();
	auto Mesh = this->Mesh.lock();

	FMatrix	ScaleMat, RotationMat, TranslateMat, WorldMat;

	ScaleMat.Scaling(Size);
	RotationMat.RotationZ(Angle);
	TranslateMat.Translation(RenderPos);

	WorldMat = ScaleMat * RotationMat * TranslateMat;

	auto World = this->World.lock();

	TransformCBuffer->SetWorldMatrix(WorldMat);
	TransformCBuffer->SetViewMatrix(FMatrix::Identity);
	TransformCBuffer->SetProjMatrix(UIProjMatrix);

	FVector PivotSize = Pivot * Mesh->GetMeshSize();
	TransformCBuffer->SetPivotSize(PivotSize);

	TransformCBuffer->UpdateBuffer();

	UIDefaultCBuffer->SetBrushTint(Brush.Tint);

	FVector2 TexSize;
	if (auto Texture = Brush.Texture.lock())
	{
		UIDefaultCBuffer->SetTextureEnable(true);

		TexSize.x = static_cast<float>(Texture->GetTexture()->Width);
		TexSize.y = static_cast<float>(Texture->GetTexture()->Height);

		if (Brush.FrameAnimationEnable)
		{
			Texture->SetShader(0, EShaderBufferType::Pixel, Brush.Frame);
		}
		else
		{
			Texture->SetShader(0, EShaderBufferType::Pixel, 0);
		}
	}
	else
	{
		UIDefaultCBuffer->SetTextureEnable(false);
	}

	if (Brush.AnimationEnable)
	{
		UIDefaultCBuffer->SetAnimEnable(true);

		int	Frame = Brush.Frame;

		FTextureFrame FrameInfo = Brush.AnimationFrames[Frame];

		UIDefaultCBuffer->SetBrushLTUV(FrameInfo.Start / TexSize);

		FVector2 FrameRB;
		FrameRB.x = FrameInfo.Start.x + FrameInfo.Size.x;
		FrameRB.y = FrameInfo.Start.y + FrameInfo.Size.y;

		UIDefaultCBuffer->SetBrushRBUV(FrameRB / TexSize);
	}
	else
	{
		UIDefaultCBuffer->SetAnimEnable(false);
	}

	UIDefaultCBuffer->UpdateBuffer();

	Shader->SetShader();

	Mesh->Render();
}

void CWidget::RenderBrush(const FUIBrush& Brush, const FVector3& RenderPos, const FVector3& Size,
	const FVector2& FrameStart, const FVector3& FrameSize)
{
	auto Shader = this->Shader.lock();
	auto Mesh = this->Mesh.lock();

	FMatrix	ScaleMat, TranslateMat, WorldMat;

	ScaleMat.Scaling(Size);
	TranslateMat.Translation(RenderPos);

	WorldMat = ScaleMat * TranslateMat;

	auto	World = this->World.lock();

	TransformCBuffer->SetWorldMatrix(WorldMat);
	TransformCBuffer->SetViewMatrix(FMatrix::Identity);
	TransformCBuffer->SetProjMatrix(UIProjMatrix);

	//FVector3	PivotSize = mPivot * Mesh->GetMeshSize();

	//mTransformCBuffer->SetPivotSize(PivotSize);

	TransformCBuffer->UpdateBuffer();

	UIDefaultCBuffer->SetBrushTint(Brush.Tint);

	FVector2 TexSize;

	if (auto Texture = Brush.Texture.lock())
	{
		UIDefaultCBuffer->SetTextureEnable(true);

		TexSize.x = static_cast<float>(Texture->GetTexture()->Width);
		TexSize.y = static_cast<float>(Texture->GetTexture()->Height);

		if (Brush.FrameAnimationEnable)
		{
			Texture->SetShader(0, EShaderBufferType::Pixel, Brush.Frame);
		}
		else
		{
			Texture->SetShader(0, EShaderBufferType::Pixel, 0);
		}
	}
	else
	{
		UIDefaultCBuffer->SetTextureEnable(false);
	}

	UIDefaultCBuffer->SetAnimEnable(true);
	UIDefaultCBuffer->SetBrushLTUV(FrameStart / TexSize);

	FVector2 FrameRB;
	FrameRB.x = FrameStart.x + FrameSize.x;
	FrameRB.y = FrameStart.y + FrameSize.y;

	UIDefaultCBuffer->SetBrushRBUV(FrameRB / TexSize);

	UIDefaultCBuffer->UpdateBuffer();

	Shader->SetShader();

	Mesh->Render();
}

void CWidget::SetParentAll()
{
}
