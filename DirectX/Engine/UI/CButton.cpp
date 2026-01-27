#include "CButton.h"

#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CCBufferUIDefault.h"
#include "../Asset/Shader/CShader.h"
#include "../Asset/Sound/CSound.h"
#include "../Asset/Texture/CTexture.h"
#include "../Asset/Texture/CTextureManager.h"
#include "../World/CInput.h"
#include "../World/CWorld.h"
#include "../World/CWorldAssetManager.h"

CButton::CButton()
{
}

CButton::~CButton()
{
}

bool CButton::SetTexture(EButtonState::Type State, const std::weak_ptr<CTexture>& Texture)
{
	Brushes[State].Texture = Texture;

	return true;
}

bool CButton::SetTexture(EButtonState::Type State, const std::string& Key)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			if (auto Texture = AssetMgr->FindTexture(Key).lock())
			{
				Brushes[State].Texture = Texture;
				return true;
			}
		}
	}

	return false;
}

bool CButton::SetTexture(EButtonState::Type State, const std::string& Key, const TCHAR* FileName,
	const std::string& PathName)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			if (!AssetMgr->LoadTexture(Key, FileName, PathName))
			{
				return false;
			}
			if (auto Texture = AssetMgr->FindTexture(Key).lock())
			{
				Brushes[State].Texture = Texture;
				return true;
			}
		}
	}

	return false;
}

void CButton::SetTint(EButtonState::Type State, const FColor& Tint)
{
	Brushes[State].Tint = Tint;
}

void CButton::SetTint(EButtonState::Type State, float r, float g, float b, float a)
{
	Brushes[State].Tint = FColor(r, g, b, a);
}

void CButton::SetBrushAnimation(EButtonState::Type State, bool Animation)
{
	Brushes[State].AnimationEnable = Animation;
}

void CButton::AddBrushFrame(EButtonState::Type State, const FVector2& Start, const FVector2& Size)
{
	auto& Brush = Brushes[State];
	Brush.AnimationFrames.emplace_back(Start, Size);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CButton::AddBrushFrame(EButtonState::Type State, float StartX, float StartY, float SizeX, float SizeY)
{
	auto& Brush = Brushes[State];
	Brush.AnimationFrames.emplace_back(StartX, StartY, SizeX, SizeY);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CButton::SetCurrentFrame(EButtonState::Type State, int Frame)
{
	Brushes[State].Frame = Frame;
}

void CButton::SetAnimationPlayTime(EButtonState::Type State, float PlayTime)
{
	auto& Brush = Brushes[State];
	Brush.PlayTime = PlayTime;

	if (!Brush.AnimationFrames.empty())
	{
		Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
	}
	else
	{
		Brush.FrameTime = FLT_MAX;
	}
}

void CButton::SetAnimationPlayRate(EButtonState::Type State, float PlayRate)
{
	Brushes[State].PlayRate = PlayRate;
}

void CButton::SetSound(EButtonEventState::Type State, const std::string& Key)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();
	Sound[State] = AssetMgr->FindSound(Key);
}

void CButton::SetSound(EButtonEventState::Type State, const std::string& Key, const char* FileName,
	const std::string& PathName)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	if (!AssetMgr->LoadSound(Key, "UI", false, FileName, PathName))
	{
		return;
	}

	Sound[State] = AssetMgr->FindSound(Key);
}

bool CButton::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	return true;
}

void CButton::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);

	if (State != EButtonState::Disable)
	{
		if (bMouseOn)
		{
			if (auto Input = World.lock()->GetInput().lock())
			{
				if (Input->GetMouseState(EMouseType::LButton, EInputType::Press))
				{
					State = EButtonState::Click;

					if (auto Sound = this->Sound[EButtonEventState::Click].lock())
					{
						Sound->Play();
					}
				}
				else if (State == EButtonState::Click
					&& Input->GetMouseState(EMouseType::LButton, EInputType::Release))
				{
					if (EventCallback[EButtonEventState::Click])
					{
						EventCallback[EButtonEventState::Click]();
					}

					State = EButtonState::Hovered;
				}

				else if (Input->GetMouseState(EMouseType::LButton, EInputType::Hold))
				{
					State = EButtonState::Click;
				}
			}
		}
	}
}

void CButton::Render()
{
	CWidget::Render();

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
	TransformCBuffer->SetProjectionMatrix(UIProjectionMatrix);

	FVector3	PivotSize = Pivot * Mesh->GetMeshSize();

	TransformCBuffer->SetPivotSize(PivotSize);

	TransformCBuffer->UpdateBuffer();

	auto& Brush = Brushes[State];
	UIDefaultCBuffer->SetBrushTint(Brush.Tint);

	if (!Brush.Texture.expired())
	{
		UIDefaultCBuffer->SetTextureEnable(true);

		auto Texture = Brush.Texture.lock();

		Texture->SetShader(0, EShaderBufferType::Pixel, 0);
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

		UIDefaultCBuffer->SetBrushLTUV(FrameInfo.Start);
		UIDefaultCBuffer->SetBrushRBUV(FrameInfo.Start + FrameInfo.Size);
	}
	else
	{
		UIDefaultCBuffer->SetAnimEnable(false);
	}

	UIDefaultCBuffer->UpdateBuffer();

	Shader->SetShader();

	Mesh->Render();

	if (Child)
	{
		Child->Render();
	}
}

bool CButton::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	if (!CWidget::CollideMouse(Result, MousePos))
	{
		return false;
	}

	if (Child)
	{
		if (!Child->CollideMouse(Result, MousePos))
		{
			return false;
		}
	}

	return true;
}

void CButton::MouseHovered()
{
	if (State == EButtonState::Normal)
	{
		if (EventCallback[EButtonEventState::Hovered])
		{
			EventCallback[EButtonEventState::Hovered]();
		}

		if (auto Sound = this->Sound[EButtonEventState::Hovered].lock())
		{
			Sound->Play();
		}

		State = EButtonState::Hovered;
	}
}

void CButton::MouseUnHovered()
{
	if (State != EButtonState::Disable)
	{
		State = EButtonState::Normal;
	}
}
