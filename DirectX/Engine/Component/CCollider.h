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
	EColliderType GetColliderType() const
	{
		return ColliderType;
	}

	bool GetDrawDebug() const
	{
		return bDrawDebug;
	}

	virtual void SetDrawDebug(bool bDrawDebug);

	FCollisionProfile* GetCollisionProfile() const
	{
		return Profile;
	}

	void SetCollisionProfile(const std::string& Key);

	bool CheckCollidingObject(CCollider* Other) const;
	void EraseCollidingObject(CCollider* Other);

	void CallOnCollisionBegin(const FVector& HitPoint, const std::weak_ptr<CCollider>& Other);
	void CallOnCollisionEnd(CCollider* Other);

	virtual bool Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other) = 0;

	template <typename T>
	void SetOnCollisionBegin(T* Obj, void (T::* Func)(const FVector&, CCollider*))
	{
		OnCollisionBegin = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}

	template <typename T>
	void SetOnCollisionEnd(T* Obj, void (T::* Func)(CCollider*))
	{
		OnCollisionEnd = std::bind(Func, Obj, std::placeholders::_1);
	}

	void SetCenterOffset(const FVector& Offset)
	{
		this->Offset = Offset;
	}

	void SetCenterOffset(float x, float y, float z)
	{
		Offset = FVector(x, y, z);
	}

protected:
	bool IsColliding() const { return CollidingObjects.empty(); }

protected:
	EColliderType ColliderType;
	FVector Min;
	FVector Max;
	FVector RenderScale;
	bool bDrawDebug = false;
	FCollisionProfile* Profile = nullptr;

	FVector Offset;

	std::unordered_map<CCollider*, std::weak_ptr<CCollider>> CollidingObjects;

	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::shared_ptr<CCBufferTransform> TransformCBuffer;
	std::shared_ptr<CCBufferCollider> ColliderCBuffer;

	std::function<void(const FVector&, CCollider*)> OnCollisionBegin;
	std::function<void(CCollider*)> OnCollisionEnd;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void Render() override;

protected:
	CCollider* Clone() const = 0;

	CCollider() = default;

	CCollider(const CCollider& other) = default;

	CCollider(CCollider&& other) noexcept
		: CSceneComponent(std::move(other)),
		ColliderType(other.ColliderType),
		Min(std::move(other.Min)),
		Max(std::move(other.Max)),
		RenderScale(std::move(other.RenderScale)),
		bDrawDebug(other.bDrawDebug),
		Profile(other.Profile),
		Shader(std::move(other.Shader)),
		Mesh(std::move(other.Mesh)),
		TransformCBuffer(std::move(other.TransformCBuffer)),
		ColliderCBuffer(std::move(other.ColliderCBuffer))
	{
		other.Profile = nullptr;
	}

	CCollider& operator=(const CCollider& other) = default;

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
		Profile = std::move(other.Profile);
		Shader = std::move(other.Shader);
		Mesh = std::move(other.Mesh);
		TransformCBuffer = std::move(other.TransformCBuffer);
		ColliderCBuffer = std::move(other.ColliderCBuffer);
		return *this;
	}

public:
	~CCollider() override;
};

