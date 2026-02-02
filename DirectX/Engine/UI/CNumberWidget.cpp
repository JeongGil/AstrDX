#include "CNumberWidget.h"

#include "../World/CWorld.h"

CNumberWidget::CNumberWidget()
{
}

CNumberWidget::CNumberWidget(const CNumberWidget& other) :
	CWidget(other)
{
	Brush = other.Brush;
	Number = other.Number;
}

CNumberWidget::~CNumberWidget()
{
}

bool CNumberWidget::SetTexture(const std::weak_ptr<CTexture>& Texture)
{
	Brush.Texture = Texture;

	return true;
}

bool CNumberWidget::SetTexture(const std::string& Key)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	auto Texture = AssetMgr->FindTexture(Key);

	return SetTexture(Texture);
}

bool CNumberWidget::SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
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

void CNumberWidget::SetTint(const FColor& Tint)
{
	Brush.Tint = Tint;
}

void CNumberWidget::SetTint(float r, float g, float b, float a)
{
	SetTint(FColor(r, g, b, a));
}

void CNumberWidget::SetBrushAnimation(bool Animation)
{
	Brush.AnimationEnable = Animation;
}

void CNumberWidget::AddBrushFrame(const FVector2& Start, const FVector2& Size)
{
	FTextureFrame	Frame;
	Frame.Start = Start;
	Frame.Size = Size;

	Brush.AnimationFrames.push_back(Frame);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CNumberWidget::AddBrushFrame(float StartX, float StartY, float SizeX, float SizeY)
{
	AddBrushFrame(FVector2(StartX, StartY), FVector2(SizeX, SizeY));
}

void CNumberWidget::SetCurrentFrame(int Frame)
{
	Brush.Frame = Frame;
}

void CNumberWidget::SetAnimationPlayTime(float PlayTime)
{
	Brush.PlayTime = PlayTime;

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CNumberWidget::SetAnimationPlayRate(float PlayRate)
{
	Brush.PlayRate = PlayRate;

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

bool CNumberWidget::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	return true;
}

void CNumberWidget::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CNumberWidget::Render()
{
	CWidget::Render();

	bool Minus = Number < 0.f;

	int	Number = static_cast<int>(this->Number);
	float Decimal = this->Number - Number;

	int	Value = 1;

	for (int i = 0; i < DecimalCount; ++i)
	{
		Value *= 10;
	}

	Decimal *= Value;

	int	DecimalNumber = static_cast<int>(Decimal);

	std::stack<int>	NumberStack;

	while (Number > 0)
	{
		auto Result = std::div(Number, 10);
		NumberStack.push(Result.rem);
		Number = Result.quot;
	}

	Numbers.clear();
	DecimalNumbers.clear();

	while (!NumberStack.empty())
	{
		Numbers.push_back(NumberStack.top());
		NumberStack.pop();
	}

	if (Numbers.empty())
	{
		Numbers.push_back(0);
	}

	while (DecimalNumber > 0)
	{
		auto Result = std::div(DecimalNumber, 10);
		NumberStack.push(Result.rem);
		DecimalNumber = Result.quot;
	}

	while (!NumberStack.empty())
	{
		DecimalNumbers.push_back(NumberStack.top());
		NumberStack.pop();
	}

	FVector WidgetSize;

	WidgetSize.y = Size.y;
	WidgetSize.x = Size.x * Numbers.size() + Size.x * DecimalNumbers.size();

	if (!DecimalNumbers.empty())
	{
		WidgetSize.x += Size.x;
	}

	if (Minus)
	{
		WidgetSize.x += Size.x;
	}

	NumberRenderPos = RenderPos;

	NumberRenderPos.y -= Size.y * 0.5f;
	NumberRenderPos.x -= WidgetSize.x * 0.5f;

	if (Minus)
	{
		FVector2 FrameStart;
		FrameStart.x = NumberSize.x * 11;
		FrameStart.y = 0.f;

		RenderBrush(Brush, NumberRenderPos, Size, FrameStart, NumberSize);

		NumberRenderPos.x += Size.x;
	}

	RenderNumber();
}

bool CNumberWidget::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	return false;
}

CNumberWidget* CNumberWidget::Clone() const
{
	return new CNumberWidget(*this);
}

void CNumberWidget::RenderNumber()
{
	FVector2 FrameStart;

	for (const auto& Number : Numbers)
	{
		FrameStart.x = NumberSize.x * Number;
		FrameStart.y = 0.f;

		RenderBrush(Brush, NumberRenderPos, this->Size, FrameStart, NumberSize);

		NumberRenderPos.x += this->Size.x;
	}

	if (!DecimalNumbers.empty())
	{
		FrameStart.x = NumberSize.x * 10;
		FrameStart.y = 0.f;

		RenderBrush(Brush, NumberRenderPos, this->Size, FrameStart, NumberSize);

		NumberRenderPos.x += this->Size.x;

		for (const auto& DecimalNumber : DecimalNumbers)

		{
			FrameStart.x = NumberSize.x * DecimalNumber;
			FrameStart.y = 0.f;

			RenderBrush(Brush, NumberRenderPos, this->Size, FrameStart, NumberSize);

			NumberRenderPos.x += this->Size.x;
		}
	}
}
