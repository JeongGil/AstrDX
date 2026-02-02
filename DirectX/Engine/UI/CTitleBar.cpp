#include "CTitleBar.h"

#include "CTextBlock.h"
#include "../Asset/Texture/CTexture.h"
#include "../World/CWorld.h"

CTitleBar::CTitleBar()
{
}

CTitleBar::CTitleBar(const CTitleBar& other) :
	CWidget(other)
{
	Brush = other.Brush;

	if (other.Child)
	{
		Child.reset(other.Child->Clone());
	}
}

CTitleBar* CTitleBar::Clone() const
{
	return new CTitleBar(*this);
}

void CTitleBar::SetParentAll()
{
	CWidget::SetParentAll();
}

void CTitleBar::SetOpacityAll(float Opacity)
{
	CWidget::SetOpacityAll(Opacity);

	if (Child)
	{
		Child->SetOpacityAll(Opacity);
	}
}

CTitleBar::~CTitleBar()
{
}

void CTitleBar::SetUpdateWidget(const std::weak_ptr<CWidget>& Widget)
{
	UpdateWidget = Widget;
}

void CTitleBar::SetText(const TCHAR* Text)
{
	if (!Child)
	{
		Child = CreateStaticWidget<CTextBlock>("Text", World);

		Child->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
		Child->SetSize(Size);
		Child->SetUIManager(UIManager);

		Child->SetText(Text);
		Child->EnableShadow(true);

		Child->SetAlignH(ETextAlignH::Center);
	}
}

void CTitleBar::SetZOrder(int ZOrder)
{
	if (auto UpdateWidget = this->UpdateWidget.lock())
	{
		UpdateWidget->SetZOrder(ZOrder);
	}
	else
	{
		CWidget::SetZOrder(ZOrder);
	}
}

void CTitleBar::ReplaceZOrder()
{
	if (auto UpdateWidget = this->UpdateWidget.lock())
	{
		UpdateWidget->ReplaceZOrder();
	}
	else
	{
		CWidget::ReplaceZOrder();
	}
}

bool CTitleBar::SetTexture(const std::weak_ptr<CTexture>& Texture)
{
	Brush.Texture = Texture;

	return true;
}

bool CTitleBar::SetTexture(const std::string& Key)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	auto Texture = AssetMgr->FindTexture(Key);

	return SetTexture(Texture);
}

bool CTitleBar::SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	if (!AssetMgr->LoadTexture(Key, FileName, PathName))
	{
		return false;
	}

	auto Texture = AssetMgr->FindTexture(Key);

	return SetTexture(Texture);
}

void CTitleBar::SetTint(const FColor& Tint)
{
	Brush.Tint = Tint;
}

void CTitleBar::SetTint(float r, float g, float b, float a)
{
	SetTint(FColor(r, g, b, a));
}

void CTitleBar::SetBrushAnimation(bool Animation)
{
	Brush.AnimationEnable = Animation;
}

void CTitleBar::AddBrushFrame(const FVector2& Start, const FVector2& Size)
{
	Brush.AnimationFrames.emplace_back(Start, Size);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CTitleBar::AddBrushFrame(float StartX, float StartY, float SizeX, float SizeY)
{
	AddBrushFrame(FVector2(StartX, StartY), FVector2(SizeX, SizeY));
}

void CTitleBar::SetCurrentFrame(int Frame)
{
	Brush.Frame = Frame;
}

void CTitleBar::SetAnimationPlayTime(float PlayTime)
{
	Brush.PlayTime = PlayTime;

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CTitleBar::SetAnimationPlayRate(float PlayRate)
{
	Brush.PlayRate = PlayRate;
}

bool CTitleBar::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	return true;
}

void CTitleBar::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);

	Brush.PlayAnimation(DeltaTime);
}

void CTitleBar::Render()
{
	CWidget::Render();

	RenderBrush(Brush, RenderPos, Size);

	if (Child)
	{
		Child->Render();
	}
}

bool CTitleBar::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	if (!CWidget::CollideMouse(Result, MousePos))
	{
		return false;
	}

	return true;
}

void CTitleBar::MouseHovered()
{
	CWidget::MouseHovered();
}

void CTitleBar::MouseUnHovered()
{
	CWidget::MouseUnHovered();
}

bool CTitleBar::MouseDragStart(const FVector2& MousePos, std::shared_ptr<CWidget>& DragOperator)
{
	CWidget::MouseDragStart(MousePos, DragOperator);

	return true;
}

bool CTitleBar::MouseDrag(const FVector2& MousePos, const FVector2& MouseMove)
{
	CWidget::MouseDrag(MousePos, MouseMove);

	if (auto UpdateWidget = this->UpdateWidget.lock())
	{
		UpdateWidget->AddPos(MouseMove);
	}
	else
	{

	}

	return true;
}

bool CTitleBar::MouseDragEnd(const FVector2& MousePos)
{
	CWidget::MouseDragEnd(MousePos);

	return true;
}
