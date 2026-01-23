#pragma once
#include "../CObject.h"

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

protected:
	CWidget();

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
	std::weak_ptr<CWorld> World;
	std::weak_ptr<CWorldUIManager> UIManager;
	std::weak_ptr<CWidget> Parent;
	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::shared_ptr<CCBufferTransform> TransformCBuffer;
	std::shared_ptr<CCBufferUIDefault> UIDefaultCBuffer;

	std::string Name;
	FVector Pos;
	FVector Size;
	FVector Pivot;
	float Angle = 0.f;

	int ZOrder = 0;

public:
	void SetShader(const std::weak_ptr<CShader>& Shader);
	void SetShader(const std::string& Key);

	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void Render();

public:
	[[nodiscard]] std::string GetName() const
	{
		return Name;
	}

	void SetName(const std::string& Name)
	{
		this->Name = Name;
	}

	[[nodiscard]] FVector GetPos() const
	{
		return Pos;
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

	void SetSize(const FVector& Size)
	{
		this->Size = Size;
	}

	void SetSize(float x, float y)
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
		this->Angle = Angle;
	}

	[[nodiscard]] int GetZOrder() const
	{
		return ZOrder;
	}

	void SetZOrder(const int ZOrder)
	{
		this->ZOrder = ZOrder;
	}
};

