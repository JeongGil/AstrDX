#include "FVector3.h"
#include "FMatrix.h"

FVector3 FVector3::Zero;
FVector3 FVector3::One = { 1.f, 1.f, 1.f };
FVector3 FVector3::Axis[EAxis::End] =
{
	{1.f, 0.f, 0.f},
	{0.f, 1.f, 0.f},
	{0.f, 0.f, 1.f}
};

#pragma region Construction

FVector3::FVector3()
{
}

FVector3::FVector3(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

FVector3::FVector3(const FVector3& v) :
	x(v.x),
	y(v.y),
	z(v.z)
{
}

FVector3::FVector3(FVector3&& v) noexcept :
	x(v.x),
	y(v.y),
	z(v.z)
{
}

FVector3::FVector3(const DirectX::XMVECTOR& v)
{
	// XMFLOAT3 to XMVECTOR
	DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(this), v);
}

#pragma endregion

#pragma region Equal

FVector3& FVector3::operator=(const FVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

FVector3& FVector3::operator=(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(this), v);
	return *this;
}

FVector3& FVector3::operator=(float Value)
{
	x = Value;
	y = Value;
	z = Value;
	return *this;
}

FVector3& FVector3::operator=(double Value)
{
	x = static_cast<float>(Value);
	y = static_cast<float>(Value);
	z = static_cast<float>(Value);
	return *this;
}

FVector3& FVector3::operator=(int Value)
{
	x = static_cast<float>(Value);
	y = static_cast<float>(Value);
	z = static_cast<float>(Value);
	return *this;
}

#pragma endregion Equal

#pragma region Plus
// +
FVector3 FVector3::operator + (const FVector3& v)	const
{
	FVector3	result;
	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;
	return result;
}

FVector3 FVector3::operator + (float Value)	const
{
	FVector3	result;
	result.x = x + Value;
	result.y = y + Value;
	result.z = z + Value;
	return result;
}

FVector3 FVector3::operator + (double Value)	const
{
	FVector3	result;
	result.x = x + static_cast<float>(Value);
	result.y = y + static_cast<float>(Value);
	result.z = z + static_cast<float>(Value);
	return result;
}

FVector3 FVector3::operator + (int Value)	const
{
	FVector3	result;
	result.x = x + static_cast<float>(Value);
	result.y = y + static_cast<float>(Value);
	result.z = z + static_cast<float>(Value);
	return result;
}

// +=
const FVector3& FVector3::operator += (const FVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

const FVector3& FVector3::operator += (float Value)
{
	x += Value;
	y += Value;
	z += Value;
	return *this;
}

const FVector3& FVector3::operator += (double Value)
{
	x += static_cast<float>(Value);
	y += static_cast<float>(Value);
	z += static_cast<float>(Value);
	return *this;
}

const FVector3& FVector3::operator += (int Value)
{
	x += static_cast<float>(Value);
	y += static_cast<float>(Value);
	z += static_cast<float>(Value);
	return *this;
}

// ++
const FVector3& FVector3::operator ++ ()
{
	x += 1.f;
	y += 1.f;
	z += 1.f;
	return *this;
}

const FVector3& FVector3::operator ++ (int)
{
	x += 1.f;
	y += 1.f;
	z += 1.f;
	return *this;
}

#pragma endregion Plus

#pragma region Minus
// -
FVector3 FVector3::operator - (const FVector3& v)	const
{
	FVector3	result;
	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;
	return result;
}

FVector3 FVector3::operator - (float Value)	const
{
	FVector3	result;
	result.x = x - Value;
	result.y = y - Value;
	result.z = z - Value;
	return result;
}

FVector3 FVector3::operator - (double Value)	const
{
	FVector3	result;
	result.x = x - static_cast<float>(Value);
	result.y = y - static_cast<float>(Value);
	result.z = z - static_cast<float>(Value);
	return result;
}

FVector3 FVector3::operator - (int Value)	const
{
	FVector3	result;
	result.x = x - static_cast<float>(Value);
	result.y = y - static_cast<float>(Value);
	result.z = z - static_cast<float>(Value);
	return result;
}

// -=
const FVector3& FVector3::operator -= (const FVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

const FVector3& FVector3::operator -= (float Value)
{
	x -= Value;
	y -= Value;
	z -= Value;
	return *this;
}

const FVector3& FVector3::operator -= (double Value)
{
	x -= static_cast<float>(Value);
	y -= static_cast<float>(Value);
	z -= static_cast<float>(Value);
	return *this;
}

const FVector3& FVector3::operator -= (int Value)
{
	x -= static_cast<float>(Value);
	y -= static_cast<float>(Value);
	z -= static_cast<float>(Value);
	return *this;
}

// --
const FVector3& FVector3::operator -- ()
{
	x -= 1.f;
	y -= 1.f;
	z -= 1.f;
	return *this;
}

const FVector3& FVector3::operator -- (int)
{
	x -= 1.f;
	y -= 1.f;
	z -= 1.f;
	return *this;
}

#pragma endregion Minus

#pragma region Multiply

// *
FVector3 FVector3::operator * (const FVector3& v)	const
{
	FVector3	result;
	result.x = x * v.x;
	result.y = y * v.y;
	result.z = z * v.z;
	return result;
}

FVector3 FVector3::operator * (float Value)	const
{
	FVector3	result;
	result.x = x * Value;
	result.y = y * Value;
	result.z = z * Value;
	return result;
}

FVector3 FVector3::operator * (double Value)	const
{
	FVector3	result;
	result.x = x * static_cast<float>(Value);
	result.y = y * static_cast<float>(Value);
	result.z = z * static_cast<float>(Value);
	return result;
}

FVector3 FVector3::operator * (int Value)	const
{
	FVector3	result;
	result.x = x * static_cast<float>(Value);
	result.y = y * static_cast<float>(Value);
	result.z = z * static_cast<float>(Value);
	return result;
}

// *=
const FVector3& FVector3::operator *= (const FVector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

const FVector3& FVector3::operator *= (float Value)
{
	x *= Value;
	y *= Value;
	z *= Value;
	return *this;
}

const FVector3& FVector3::operator *= (double Value)
{
	x *= static_cast<float>(Value);
	y *= static_cast<float>(Value);
	z *= static_cast<float>(Value);
	return *this;
}

const FVector3& FVector3::operator *= (int Value)
{
	x *= static_cast<float>(Value);
	y *= static_cast<float>(Value);
	z *= static_cast<float>(Value);
	return *this;
}

#pragma endregion Multiply

#pragma region Divide

// /
FVector3 FVector3::operator / (const FVector3& v)	const
{
	FVector3	result;
	result.x = x / v.x;
	result.y = y / v.y;
	result.z = z / v.z;
	return result;
}

FVector3 FVector3::operator / (float Value)	const
{
	FVector3	result;
	result.x = x / Value;
	result.y = y / Value;
	result.z = z / Value;
	return result;
}

FVector3 FVector3::operator / (double Value)	const
{
	FVector3	result;
	result.x = x / static_cast<float>(Value);
	result.y = y / static_cast<float>(Value);
	result.z = z / static_cast<float>(Value);
	return result;
}

FVector3 FVector3::operator / (int Value)	const
{
	FVector3	result;
	result.x = x / static_cast<float>(Value);
	result.y = y / static_cast<float>(Value);
	result.z = z / static_cast<float>(Value);
	return result;
}

// /=
const FVector3& FVector3::operator /= (const FVector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

const FVector3& FVector3::operator /= (float Value)
{
	x /= Value;
	y /= Value;
	z /= Value;
	return *this;
}

const FVector3& FVector3::operator /= (double Value)
{
	x /= static_cast<float>(Value);
	y /= static_cast<float>(Value);
	z /= static_cast<float>(Value);
	return *this;
}

const FVector3& FVector3::operator /= (int Value)
{
	x /= static_cast<float>(Value);
	y /= static_cast<float>(Value);
	z /= static_cast<float>(Value);
	return *this;
}

#pragma endregion Divide

#pragma region Function

float FVector3::Length()	const
{
	// sqrtf : calc root value function.
	return sqrtf(x * x + y * y + z * z);
}

void FVector3::Normalize()
{
	float	Size = Length();

	if (Size == 0.f)
		return;

	x /= Size;
	y /= Size;
	z /= Size;
}


FVector3 FVector3::Normalize(const FVector3& v)
{
	FVector3	result;
	float	Size = v.Length();

	if (Size == 0.f)
		return result;

	result.x = v.x / Size;
	result.y = v.y / Size;
	result.z = v.z / Size;
	return result;
}

float FVector3::Dot(const FVector3& v)	const
{
	return x * v.x + y * v.y + z * v.z;
}

FVector3 FVector3::Cross(const FVector3& v)	const
{
	FVector3	result;
	result.x = y * v.z - z * v.y;
	result.y = z * v.x - x * v.z;
	result.z = x * v.y - y * v.x;
	return result;
}

float FVector3::Distance(const FVector3& v)	const
{
	FVector3 v1 = *this - v;

	return v1.Length();
}

DirectX::XMVECTOR FVector3::Convert()	const
{
	return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
}

//FVector3 FVector3::TransformNormal(FMatrix& mat)	const
//{
//	// XMVector3TransformNormal : It sets w to 0, multiplies it by a 4x4 matrix, and returns the result.
//	DirectX::XMVECTOR result = DirectX::XMVector3TransformNormal(Convert(),
//		mat.m);
//
//	return result;
//}
//
//FVector3 FVector3::TransformCoord(FMatrix& mat)	const
//{
//	// XMVector3TransformCoord : It sets w to 1, multiplies it by a 4x4 matrix, and returns the result.
//	DirectX::XMVECTOR result = DirectX::XMVector3TransformCoord(Convert(),
//		mat.m);
//
//	return result;
//}
//
//FVector3 FVector3::GetRotation(const FVector3& Rot)	const
//{
//	FMatrix	matRot;
//
//	matRot.Rotation(Rot);
//
//	return TransformNormal(matRot);
//}
//
//float FVector3::GetAngle(const FVector3& v) const
//{
//	FVector3	v1 = *this;
//	FVector3	v2 = v;
//
//	v1.Normalize();
//	v2.Normalize();
//
//	float Angle = v1.Dot(v2);
//
//	Angle = DirectX::XMConvertToDegrees(acosf(Angle));
//
//	return Angle;
//}
//
//float FVector3::GetViewTargetAngle(const FVector3& v,
//	EAxis::Type AxisType)	const
//{
//	FVector3	v1 = Axis[AxisType];
//	FVector3	v2 = v - *this;
//
//	v1.Normalize();
//	v2.Normalize();
//
//	float Angle = v1.Dot(v2);
//
//	Angle = DirectX::XMConvertToDegrees(acosf(Angle));
//
//	if (v.x > x)
//		Angle = 360.f - Angle;
//
//	return Angle;
//}
//
//float FVector3::GetAngle(const FVector3& v1, const FVector3& v2)
//{
//	FVector3	v3 = v1;
//	FVector3	v4 = v2;
//
//	v3.Normalize();
//	v4.Normalize();
//
//	float Angle = v3.Dot(v4);
//
//	Angle = DirectX::XMConvertToDegrees(acosf(Angle));
//
//	return Angle;
//}

#pragma endregion Function
