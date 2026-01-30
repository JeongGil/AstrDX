#pragma once
#include "CWidget.h"

class CSound;

namespace EButtonState
{
	enum Type
	{
		Normal,
		Hovered,
		Click,
		Disable,
		End
	};
}

namespace EButtonEventState
{
	enum Type
	{
		Hovered,
		Click,
		End
	};
}

class CWorldUIManager;
class CWidgetContainer;
class CTexture;

class CButton :
	public CWidget
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;

protected:
	CButton();
	CButton(const CButton& other);

public:
	~CButton() override;

protected:
	std::shared_ptr<CWidget> Child;
	FUIBrush Brush[EButtonState::End];
	EButtonState::Type State = EButtonState::Normal;

	std::weak_ptr<CSound> Sound[EButtonEventState::End];
	std::function<void()> EventCallback[EButtonEventState::End];

public:
	void ButtonEnable(bool bEnable)
	{
		State = bEnable ? EButtonState::Normal : EButtonState::Disable;
	}

	void SetChild(const std::weak_ptr<CWidget>& Child)
	{
		if (auto SChild = Child.lock())
		{
			SChild->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
			SChild->SetSize(Size);

			this->Child = SChild;

			this->Child->SetUIManager(UIManager);
			this->Child->SetWorld(World);
		}
		else
		{
			this->Child.reset();
		}
	}

public:
	bool SetTexture(EButtonState::Type State, const std::weak_ptr<CTexture>& Texture);
	bool SetTexture(EButtonState::Type State, const std::string& Key);
	bool SetTexture(EButtonState::Type State, const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	void SetTint(EButtonState::Type State, const FColor& Tint);
	void SetTint(EButtonState::Type State, float r, float g, float b, float a);
	void SetBrushAnimation(EButtonState::Type State, bool Animation);
	void AddBrushFrame(EButtonState::Type State, const FVector2& Start, const FVector2& Size);
	void AddBrushFrame(EButtonState::Type State, float StartX, float StartY, float SizeX, float SizeY);
	void SetCurrentFrame(EButtonState::Type State, int Frame);
	void SetAnimationPlayTime(EButtonState::Type State, float PlayTime);
	void SetAnimationPlayRate(EButtonState::Type State, float PlayRate);
	void SetSound(EButtonEventState::Type State, const std::string& Key);
	void SetSound(EButtonEventState::Type State, const std::string& Key, const char* FileName, const std::string& PathName = "Sound");
	void SetParentAll() override;

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
	bool CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos) override;
	void MouseHovered() override;
	void MouseUnHovered() override;
	CButton* Clone() const override;

	template <typename T>
	void SetEventCallback(EButtonEventState::Type Type, T* Obj, void(T::* Func)())
	{
		EventCallback[Type] = std::bind(Func, Obj);
	}
};
