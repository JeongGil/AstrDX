#pragma once

#include "FVector4.h"
#include "FVector3.h"
#include "FVector2.h"
#include <memory>

// union: share memory between members
// __declspec(align(16)): align memory to 16 bytes boundary
__declspec(align(16)) union FMatrix
{
	DirectX::XMMATRIX	m;

	struct
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};

	FVector4 v[4];

	FMatrix()
	{
		// make identity matrix
		m = DirectX::XMMatrixIdentity();
	}

	FMatrix(const FMatrix& _m)
	{
		m = _m.m;
	}

	FMatrix(FMatrix&& _m) noexcept
	{
		m = _m.m;
	}

	FMatrix(const DirectX::XMMATRIX& _m)
	{
		m = _m;
	}

	FMatrix(const FVector4 _v[4])
	{
		memcpy(v, _v, sizeof(FVector4) * 4);
	}

	FVector4& operator [] (int Index)
	{
		assert(0 <= Index && Index <= 3);

		return v[Index];
	}

	FMatrix& operator = (const FMatrix& _m)
	{
		if (this == &_m)
			return *this;
		m = _m.m;
		return *this;
	}

	FMatrix& operator = (const DirectX::XMMATRIX& _m)
	{
		m = _m;
		return *this;
	}

	FMatrix& operator = (const FVector4 _v[4])
	{
		memcpy(v, _v, sizeof(FVector4) * 4);
		return *this;
	}

	FMatrix operator * (const FMatrix& _m)	const
	{
		return { m * _m.m };
	}

	FMatrix operator * (const DirectX::XMMATRIX& _m)	const
	{
		return { m * _m };
	}

	void SetIdentity()
	{
		m = DirectX::XMMatrixIdentity();
	}

	void Transpose()
	{
		m = DirectX::XMMatrixTranspose(m);
	}

	void Inverse()
	{
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(m);

		m = DirectX::XMMatrixInverse(&det, m);
	}

	// scaling per x, y, z
	void Scaling(const FVector& _v)
	{
		m = DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
	}

	void Scaling(float x, float y, float z)
	{
		m = DirectX::XMMatrixScaling(x, y, z);
	}

	void Scaling(const FVector2& _v)
	{
		m = DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
	}

	void Scaling(float x, float y)
	{
		m = DirectX::XMMatrixScaling(x, y, 1.f);
	}

	void Rotation(const FVector& eulerDegree)
	{
		float x = DirectX::XMConvertToRadians(eulerDegree.x);
		float y = DirectX::XMConvertToRadians(eulerDegree.y);
		float z = DirectX::XMConvertToRadians(eulerDegree.z);

		// make quaternion from euler angle
		DirectX::XMVECTOR Quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// make rot matrix from quaternion
		m = DirectX::XMMatrixRotationQuaternion(Quat);
	}

	void Rotation(float _x, float _y, float _z)
	{
		float x = DirectX::XMConvertToRadians(_x);
		float y = DirectX::XMConvertToRadians(_y);
		float z = DirectX::XMConvertToRadians(_z);

		DirectX::XMVECTOR Quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		m = DirectX::XMMatrixRotationQuaternion(Quat);
	}

	void RotationX(float degree)
	{
		float x = DirectX::XMConvertToRadians(degree);

		m = DirectX::XMMatrixRotationX(x);
	}

	void RotationY(float degree)
	{
		float y = DirectX::XMConvertToRadians(degree);

		m = DirectX::XMMatrixRotationY(y);
	}

	void RotationZ(float degree)
	{
		float z = DirectX::XMConvertToRadians(degree);

		m = DirectX::XMMatrixRotationZ(z);
	}

	void RotationAxis(const FVector& Axis, float degree)
	{
		float radian = DirectX::XMConvertToRadians(degree);

		DirectX::XMVECTOR _Axis = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&Axis);

		// rot radian with _Axis
		m = DirectX::XMMatrixRotationAxis(_Axis, radian);
	}

	void Translation(const FVector& _v)
	{
		m = DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
	}

	void Translation(float x, float y, float z)
	{
		m = DirectX::XMMatrixTranslation(x, y, z);
	}

	void Translation(const FVector2& _v)
	{
		m = DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
	}

	void Translation(float x, float y)
	{
		m = DirectX::XMMatrixTranslation(x, y, 0.f);
	}

	static FMatrix StaticIdentity()
	{
		return DirectX::XMMatrixIdentity();
	}

	static FMatrix StaticTranspose(const FMatrix& _m)
	{
		return DirectX::XMMatrixTranspose(_m.m);
	}

	static FMatrix StaticInverse(const FMatrix& _m)
	{
		DirectX::XMVECTOR	det = DirectX::XMMatrixDeterminant(_m.m);
		return DirectX::XMMatrixInverse(&det, _m.m);
	}

	static FMatrix StaticScaling(const FVector& _v)
	{
		return DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
	}

	static FMatrix StaticScaling(float x, float y, float z)
	{
		return DirectX::XMMatrixScaling(x, y, z);
	}

	static FMatrix StaticScaling(const FVector2& _v)
	{
		return DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
	}

	static FMatrix StaticScaling(float x, float y)
	{
		return DirectX::XMMatrixScaling(x, y, 1.f);
	}

	static FMatrix StaticRotation(const FVector& eulerDegree)
	{
		float x = DirectX::XMConvertToRadians(eulerDegree.x);
		float y = DirectX::XMConvertToRadians(eulerDegree.y);
		float z = DirectX::XMConvertToRadians(eulerDegree.z);

		DirectX::XMVECTOR Quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		return DirectX::XMMatrixRotationQuaternion(Quat);
	}

	static FMatrix StaticRotation(float _x, float _y, float _z)
	{
		float x = DirectX::XMConvertToRadians(_x);
		float y = DirectX::XMConvertToRadians(_y);
		float z = DirectX::XMConvertToRadians(_z);

		DirectX::XMVECTOR Quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		return DirectX::XMMatrixRotationQuaternion(Quat);
	}

	static FMatrix StaticRotationX(float _x)
	{
		float x = DirectX::XMConvertToRadians(_x);

		return DirectX::XMMatrixRotationX(x);
	}

	static FMatrix StaticRotationY(float _y)
	{
		float y = DirectX::XMConvertToRadians(_y);

		return DirectX::XMMatrixRotationY(y);
	}

	static FMatrix StaticRotationZ(float _z)
	{
		float z = DirectX::XMConvertToRadians(_z);

		return DirectX::XMMatrixRotationZ(z);
	}

	static FMatrix StaticRotationAxis(const FVector& Axis, float degree)
	{
		float radian = DirectX::XMConvertToRadians(degree);

		DirectX::XMVECTOR _Axis = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&Axis);

		return DirectX::XMMatrixRotationAxis(_Axis, radian);
	}

	static FMatrix StaticTranslation(const FVector& _v)
	{
		return DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
	}

	static FMatrix StaticTranslation(float x, float y, float z)
	{
		return DirectX::XMMatrixTranslation(x, y, z);
	}

	static FMatrix StaticTranslation(const FVector2& _v)
	{
		return DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
	}

	static FMatrix StaticTranslation(float x, float y)
	{
		return DirectX::XMMatrixTranslation(x, y, 0.f);
	}

	static FMatrix Identity;

	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}
};
