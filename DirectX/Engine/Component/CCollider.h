#pragma once
#include "CSceneComponent.h"

class CCBufferCollider;
class CCBufferTransform;
class CMesh;
class CShader;

class CCollider abstract :
	public CSceneComponent
{
public:
	EColliderType GetColliderType() const { return ColliderType; }

	bool GetDrawDebug() const { return bDrawDebug; }
	virtual void SetDrawDebug(bool bDrawDebug);

protected:
	EColliderType ColliderType;
	FVector Min;
	FVector Max;
	FVector RenderScale;
	bool bDrawDebug = false;
	bool bCollide = false;

	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::shared_ptr<CCBufferTransform> TransformCBuffer;
	std::shared_ptr<CCBufferCollider> ColliderCBuffer;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void Render() override;

protected:
	CCollider* Clone() const = 0;

	CCollider() = default;

	CCollider(const CCollider& other)
		: CSceneComponent(other),
		  ColliderType(other.ColliderType),
		  Min(other.Min),
		  Max(other.Max),
		  RenderScale(other.RenderScale),
		  bDrawDebug(other.bDrawDebug),
		  bCollide(other.bCollide),
		  Shader(other.Shader),
		  Mesh(other.Mesh),
		  TransformCBuffer(other.TransformCBuffer),
		  ColliderCBuffer(other.ColliderCBuffer)
	{
	}

	CCollider(CCollider&& other) noexcept
		: CSceneComponent(std::move(other)),
		  ColliderType(other.ColliderType),
		  Min(std::move(other.Min)),
		  Max(std::move(other.Max)),
		  RenderScale(std::move(other.RenderScale)),
		  bDrawDebug(other.bDrawDebug),
		  bCollide(other.bCollide),
		  Shader(std::move(other.Shader)),
		  Mesh(std::move(other.Mesh)),
		  TransformCBuffer(std::move(other.TransformCBuffer)),
		  ColliderCBuffer(std::move(other.ColliderCBuffer))
	{
	}

	CCollider& operator=(const CCollider& other)
	{
		if (this == &other)
			return *this;
		CSceneComponent::operator =(other);
		ColliderType = other.ColliderType;
		Min = other.Min;
		Max = other.Max;
		RenderScale = other.RenderScale;
		bDrawDebug = other.bDrawDebug;
		bCollide = other.bCollide;
		Shader = other.Shader;
		Mesh = other.Mesh;
		TransformCBuffer = other.TransformCBuffer;
		ColliderCBuffer = other.ColliderCBuffer;
		return *this;
	}

	CCollider& operator=(CCollider&& other) noexcept
	{
		if (this == &other)
			return *this;
		CSceneComponent::operator =(std::move(other));
		ColliderType = other.ColliderType;
		Min = std::move(other.Min);
		Max = std::move(other.Max);
		RenderScale = std::move(other.RenderScale);
		bDrawDebug = other.bDrawDebug;
		bCollide = other.bCollide;
		Shader = std::move(other.Shader);
		Mesh = std::move(other.Mesh);
		TransformCBuffer = std::move(other.TransformCBuffer);
		ColliderCBuffer = std::move(other.ColliderCBuffer);
		return *this;
	}

public:
	~CCollider() override = default;
};

