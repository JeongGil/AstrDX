#pragma once

#include "EngineMath.h"

struct FVector3
{
	float	x = 0.f;
	float	y = 0.f;
	float	z = 0.f;

#pragma region Construction

	FVector3();
	FVector3(float _x, float _y, float _z);
	FVector3(const FVector3& v);
	FVector3(FVector3&& v) noexcept;
	FVector3(const DirectX::XMVECTOR& v);

#pragma endregion Construction

#pragma region Equal

	FVector3& operator=(const FVector3& v);
	FVector3& operator=(const DirectX::XMVECTOR& v);
	FVector3& operator=(float Value);
	FVector3& operator=(double Value);
	FVector3& operator=(int Value);

#pragma endregion Equal

#pragma region Plus
	// +
	FVector3 operator+(const FVector3& v)	const;
	FVector3 operator+(float Value)	const;
	FVector3 operator+(double Value)	const;
	FVector3 operator+(int Value)	const;

	// +=
	const FVector3& operator+=(const FVector3& v);
	const FVector3& operator+=(float Value);
	const FVector3& operator+=(double Value);
	const FVector3& operator+=(int Value);

	// ++
	const FVector3& operator++();
	const FVector3& operator++(int);

#pragma endregion Plus

#pragma region Minus
	// -
	FVector3 operator-(const FVector3& v) const;
	FVector3 operator-(float Value) const;
	FVector3 operator-(double Value) const;
	FVector3 operator-(int Value) const;

	// -=
	const FVector3& operator-=(const FVector3& v);
	const FVector3& operator-=(float Value);
	const FVector3& operator-=(double Value);
	const FVector3& operator-=(int Value);

	// --
	const FVector3& operator--();
	const FVector3& operator--(int);

#pragma endregion

#pragma region Multiply

	// *
	FVector3 operator*(const FVector3& v) const;
	FVector3 operator*(float Value) const;
	FVector3 operator*(double Value) const;
	FVector3 operator*(int Value) const;

	// *=
	const FVector3& operator*=(const FVector3& v);
	const FVector3& operator*=(float Value);
	const FVector3& operator*=(double Value);
	const FVector3& operator*=(int Value);

#pragma endregion Multiply

#pragma region Divide

	// /
	FVector3 operator/(const FVector3& v) const;
	FVector3 operator/(float Value) const;
	FVector3 operator/(double Value) const;
	FVector3 operator/(int Value) const;

	// /=
	const FVector3& operator/=(const FVector3& v);
	const FVector3& operator/=(float Value);
	const FVector3& operator/=(double Value);
	const FVector3& operator/=(int Value);

#pragma endregion

#pragma region Function

	float Length()	const;
	float SqrLength() const;

	void Normalize();
	static FVector3 Normalize(const FVector3& v);
	FVector3 GetNormalized() const
	{
		return Normalize(*this);
	}

	float Dot(const FVector3& v) const;
	FVector3 Cross(const FVector3& v) const;

	float Distance(const FVector3& v) const;
	float SqrDistance(const FVector3& v) const;

	DirectX::XMVECTOR Convert()	const;

	// w = 0. Only apply scale and rotation, not position.
	FVector3 TransformNormal(union FMatrix& mat) const;
	
	// w=w 1. Apply scale, rotation and Position.
	FVector3 TransformCoord(union FMatrix& mat)	const;
	
	float GetAngleDegree2D(const FVector3& v) const;
	float GetViewTargetAngleDegree2D(const FVector3& v, EAxis::Type AxisType = EAxis::Y) const;
	static float GetAngleDegree2D(const FVector3& v1, const FVector3& v2);

	//FVector3 GetRotation(const FVector3& Rot) const;
	//
	//float GetAngle(const FVector3& v)	const;
	//float GetViewTargetAngle(const FVector3& v, EAxis::Type AxisType = EAxis::Y) const;
	//static float GetAngle(const FVector3& v1, const FVector3& v2);

#pragma endregion

#pragma region StaticVariable

	static FVector3	Zero;
	static FVector3	One;
	static FVector3	Axis[EAxis::End];

#pragma endregion
};

using FVector = FVector3;
