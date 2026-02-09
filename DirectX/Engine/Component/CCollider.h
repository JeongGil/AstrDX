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
	const FVector& GetMin() const
	{
		return Min;
	}

	const FVector& GetMax() const
	{
		return Max;
	}

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

	bool GetStatic() const
	{
		return bStatic;
	}

	void SetStatic(bool bStatic)
	{
		this->bStatic = bStatic;
	}

	bool CheckCollidingObject(CCollider* Other) const;
	void EraseCollidingObject(CCollider* Other);

	void CallOnCollisionBegin(const FVector& HitPoint, const std::weak_ptr<CCollider>& Other);
	void CallOnCollisionEnd(CCollider* Other);
	void CallOnCollisionBlock(const FVector& HitPoint, const std::weak_ptr<CCollider>& Other);
	void OnCollisionBlockEnd();
	void CallOnCollisionMouseBegin(const FVector& MousePos);
	void CallOnCollisionMouseEnd(const FVector& MousePos);

	virtual bool Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other) = 0;
	virtual bool CollideManifold(FCollisionManifold& HitResult, std::shared_ptr<CCollider> Dest) = 0;
	virtual bool CollideMouse(const FVector2& MousePos) = 0;

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

	template <typename T>
	void SetOnCollisionBlock(T* Obj, void (T::* Func)(const FVector&, CCollider*))
	{
		OnCollisionBlock = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}

	template <typename T>
	void SetOnCollisionMouseBegin(T* Obj, void (T::* Func)(const FVector&))
	{
		OnCollisionMouseBegin = std::bind(Func, Obj, std::placeholders::_1);
	}

	template <typename T>
	void SetOnCollisionMouseEnd(T* Obj, void (T::* Func)(const FVector&))
	{
		OnCollisionMouseEnd = std::bind(Func, Obj, std::placeholders::_1);
	}

protected:
	bool IsColliding() const
	{
		return !CollidingObjects.empty();
	}

protected:
	EColliderType ColliderType;
	FVector Min = FVector::Zero;
	FVector Max = FVector::Zero;
	FVector RenderScale = FVector::One;
	bool bDrawDebug = false;
	bool bStatic = false;
	FCollisionProfile* Profile = nullptr;

	std::unordered_map<CCollider*, std::weak_ptr<CCollider>> CollidingObjects;

	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::shared_ptr<CCBufferTransform> TransformCBuffer;
	std::shared_ptr<CCBufferCollider> ColliderCBuffer;

	std::function<void(const FVector&, CCollider*)> OnCollisionBegin;
	std::function<void(CCollider*)> OnCollisionEnd;
	std::function<void(const FVector&, CCollider*)> OnCollisionBlock;

	std::function<void(const FVector&)> OnCollisionMouseBegin;
	std::function<void(const FVector&)> OnCollisionMouseEnd;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void Render() override;
	virtual void UpdateInfo() = 0;

protected:
	CCollider* Clone() const override = 0;

	CCollider() = default;
	CCollider(const CCollider& other) = default;
	CCollider(CCollider&& other) noexcept;

public:
	~CCollider() override;
};

