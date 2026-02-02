#pragma once
#include "../CObject.h"

#include "UIInfo.h"

class CWidgetContainer;
class CCBufferUIDefault;
class CCBufferTransform;
class CMesh;
class CShader;
class CWorld;
class CWorldUIManager;

class CWidget :
	public CObject
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	static bool SortRender(const std::shared_ptr<CWidget>& A, const std::shared_ptr<CWidget>& B);
	static bool SortCollision(const std::shared_ptr<CWidget>& A, const std::shared_ptr<CWidget>& B);

protected:
	CWidget();
	CWidget(const CWidget& other);

public:
	virtual ~CWidget() = 0;

protected:
	inline static FMatrix UIProjectionMatrix;

public:
	static void CreateUIProjection(float Width, float Height)
	{
		UIProjectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(
			0.f, Width, Height, 0.f, 0.f, 1000.f);
	}

protected:
	bool bEnable = true;
	bool bAlive = true;

	std::weak_ptr<CWorld> World;
	std::weak_ptr<CWorldUIManager> UIManager;
	std::weak_ptr<CWidget> Parent;
	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::shared_ptr<CCBufferTransform> TransformCBuffer;
	std::shared_ptr<CCBufferUIDefault> UIDefaultCBuffer;

	std::string Key;
	FVector Pos;
	FVector RenderPos;
	FVector Size;
	FVector Pivot;
	float Angle = 0.f;

	int ZOrder = 0;
	int PrevZOrder = 0;

	bool bMouseOn = false;
	bool bMouseDrag = false;

	FColor WidgetColor = FColor::White;

public:
	void SetShader(const std::weak_ptr<CShader>& Shader);
	void SetShader(const std::string& Key);

	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void Render();
	virtual void Destroy();
	virtual bool CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos);
	virtual void MouseHovered();
	virtual void MouseUnHovered();
	virtual bool MouseDragStart(const FVector2& MousePos, std::shared_ptr<CWidget>& DragOperator);
	virtual bool MouseDrag(const FVector2& MousePos, const FVector2& MouseMove);
	virtual bool MouseDragEnd(const FVector2& MousePos);
	virtual CWidget* Clone() const = 0;

	template <typename T>
	static std::shared_ptr<T> CreateStaticWidget(const std::string& Key, std::weak_ptr<CWorld> World, int ZOrder = 0)
	{
		std::shared_ptr<T> Widget;
		Widget.reset(new T);

		Widget->World = World;
		Widget->SetKey(Key);
		Widget->SetZOrder(ZOrder);

		if (!Widget->Init())
		{
			return {};
		}

		return std::dynamic_pointer_cast<T>(Widget);
	}

protected:
	void RenderBrush(const FUIBrush& Brush, const FVector& RenderPos, const FVector& Size);
	void RenderBrush(const FUIBrush& Brush, const FVector3& RenderPos, const FVector3& Size, const FVector2& FrameStart, const FVector3& FrameSize);

public:
	[[nodiscard]] bool GetMouseDrag() const
	{
		return bMouseDrag;
	}

	void SetMouseDragEnable(bool bEnable)
	{
		bMouseDrag = bEnable;
	}

	void SetWorld(const std::weak_ptr<CWorld>& World)
	{
		this->World = World;
	}

	void SetUIManager(const std::weak_ptr<CWorldUIManager>& UIManager)
	{
		this->UIManager = UIManager;
	}

	[[nodiscard]] std::string GetKey() const
	{
		return Key;
	}

	void SetKey(const std::string& Key)
	{
		this->Key = Key;
	}

	void SetParent(const std::weak_ptr<CWidget>& Parent)
	{
		this->Parent = Parent;
	}

	virtual void SetParentAll();

	[[nodiscard]] FVector GetPos() const
	{
		return Pos;
	}

	void AddPos(const FVector2& Move)
	{
		Pos.x += Move.x;
		Pos.y += Move.y;
	}

	void SetPos(const FVector& Pos)
	{
		this->Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		Pos.x = x;
		Pos.y = y;
	}

	[[nodiscard]] FVector GetSize() const
	{
		return Size;
	}

	virtual void SetSize(const FVector& Size)
	{
		this->Size = Size;
	}

	virtual void SetSize(float x, float y)
	{
		Size.x = x;
		Size.y = y;
	}

	[[nodiscard]] FVector GetPivot() const
	{
		return Pivot;
	}

	void SetPivot(const FVector& Pivot)
	{
		this->Pivot = Pivot;
	}

	void SetPivot(float x, float y)
	{
		Pivot.x = x;
		Pivot.y = y;
	}

	[[nodiscard]] float GetAngle() const
	{
		return Angle;
	}

	void SetAngle(const float Angle)
	{
		this->Angle = std::fmod(Angle, 360.f);

		if (this->Angle < 0.f)
		{
			this->Angle = +360.f;
		}
	}

	[[nodiscard]] int GetZOrder() const
	{
		return ZOrder;
	}

	virtual void SetZOrder(const int ZOrder)
	{
		PrevZOrder = this->ZOrder;
		this->ZOrder = ZOrder;
	}

	virtual void ReplaceZOrder()
	{
		this->ZOrder = PrevZOrder;
	}

	[[nodiscard]] FColor GetWidgetColor() const
	{
		return WidgetColor;
	}

	void SetWidgetColor(const FColor& WidgetColor)
	{
		this->WidgetColor = WidgetColor;
	}

	void SetWidgetColor(float r, float g, float b, float a)
	{
		this->WidgetColor = FColor(r, g, b, a);
	}

	virtual void SetOpacity(float Opacity)
	{
		WidgetColor.w = std::clamp(Opacity, 0.f, 1.f);
	}

	virtual void SetOpacityAll(float Opacity)
	{
		WidgetColor.w = std::clamp(Opacity, 0.f, 1.f);
	}

	[[nodiscard]] bool GetEnable() const
	{
		return bEnable;
	}

	void SetEnable(const bool bEnable)
	{
		this->bEnable = bEnable;
	}

	[[nodiscard]] bool GetAlive() const
	{
		return bAlive;
	}
};

