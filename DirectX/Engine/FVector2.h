#pragma once

#include "EngineMath.h"

struct FVector2
{
	float	x = 0.f;
	float	y = 0.f;

#pragma region Construction

	FVector2() = default;

	FVector2(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}

	FVector2(const FVector2& v) = default;

	FVector2(FVector2&& v) noexcept :
		x(v.x),
		y(v.y)
	{
	}

#pragma endregion Construction

#pragma region Equal

	FVector2& operator = (const FVector2& v) = default;

	FVector2& operator = (float Value)
	{
		x = Value;
		y = Value;
		return *this;
	}

	FVector2& operator = (double Value)
	{
		x = static_cast<float>(Value);
		y = static_cast<float>(Value);
		return *this;
	}

	FVector2& operator = (int Value)
	{
		x = static_cast<float>(Value);
		y = static_cast<float>(Value);
		return *this;
	}

#pragma endregion Equal

#pragma region Plus
	// +
	FVector2 operator + (const FVector2& v)	const
	{
		FVector2	result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	FVector2 operator + (float Value)	const
	{
		FVector2	result;
		result.x = x + Value;
		result.y = y + Value;
		return result;
	}

	FVector2 operator + (double Value)	const
	{
		FVector2	result;
		result.x = x + static_cast<float>(Value);
		result.y = y + static_cast<float>(Value);
		return result;
	}

	FVector2 operator + (int Value)	const
	{
		FVector2	result;
		result.x = x + static_cast<float>(Value);
		result.y = y + static_cast<float>(Value);
		return result;
	}

	// +=
	const FVector2& operator += (const FVector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	const FVector2& operator += (float Value)
	{
		x += Value;
		y += Value;
		return *this;
	}

	const FVector2& operator += (double Value)
	{
		x += static_cast<float>(Value);
		y += static_cast<float>(Value);
		return *this;
	}

	const FVector2& operator += (int Value)
	{
		x += static_cast<float>(Value);
		y += static_cast<float>(Value);
		return *this;
	}

	// ++
	const FVector2& operator ++ ()
	{
		x += 1.f;
		y += 1.f;
		return *this;
	}

	const FVector2& operator ++ (int)
	{
		x += 1.f;
		y += 1.f;
		return *this;
	}

#pragma endregion Plus

#pragma region Minus
	// -
	FVector2 operator - (const FVector2& v)	const
	{
		FVector2 result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	FVector2 operator - (float Value)	const
	{
		FVector2 result;
		result.x = x - Value;
		result.y = y - Value;
		return result;
	}

	FVector2 operator - (double Value)	const
	{
		FVector2 result;
		result.x = x - static_cast<float>(Value);
		result.y = y - static_cast<float>(Value);
		return result;
	}

	FVector2 operator - (int Value)	const
	{
		FVector2 result;
		result.x = x - static_cast<float>(Value);
		result.y = y - static_cast<float>(Value);
		return result;
	}

	// -=
	const FVector2& operator -= (const FVector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const FVector2& operator -= (float Value)
	{
		x -= Value;
		y -= Value;
		return *this;
	}

	const FVector2& operator -= (double Value)
	{
		x -= static_cast<float>(Value);
		y -= static_cast<float>(Value);
		return *this;
	}

	const FVector2& operator -= (int Value)
	{
		x -= static_cast<float>(Value);
		y -= static_cast<float>(Value);
		return *this;
	}

	// --
	const FVector2& operator -- ()
	{
		x -= 1.f;
		y -= 1.f;
		return *this;
	}

	const FVector2& operator -- (int)
	{
		x -= 1.f;
		y -= 1.f;
		return *this;
	}

#pragma endregion Minus

#pragma region Multiply

	// *
	FVector2 operator * (const FVector2& v)	const
	{
		FVector2	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	FVector2 operator * (float Value)	const
	{
		FVector2	result;
		result.x = x * Value;
		result.y = y * Value;
		return result;
	}

	FVector2 operator * (double Value)	const
	{
		FVector2	result;
		result.x = x * static_cast<float>(Value);
		result.y = y * static_cast<float>(Value);
		return result;
	}

	FVector2 operator * (int Value)	const
	{
		FVector2	result;
		result.x = x * static_cast<float>(Value);
		result.y = y * static_cast<float>(Value);
		return result;
	}

	// *=
	const FVector2& operator *= (const FVector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const FVector2& operator *= (float Value)
	{
		x *= Value;
		y *= Value;
		return *this;
	}

	const FVector2& operator *= (double Value)
	{
		x *= static_cast<float>(Value);
		y *= static_cast<float>(Value);
		return *this;
	}

	const FVector2& operator *= (int Value)
	{
		x *= static_cast<float>(Value);
		y *= static_cast<float>(Value);
		return *this;
	}

#pragma endregion Multiply

#pragma region Divide

	// /
	FVector2 operator / (const FVector2& v)	const
	{
		FVector2	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	FVector2 operator / (float Value)	const
	{
		FVector2	result;
		result.x = x / Value;
		result.y = y / Value;
		return result;
	}

	FVector2 operator / (double Value)	const
	{
		FVector2	result;
		result.x = x / static_cast<float>(Value);
		result.y = y / static_cast<float>(Value);
		return result;
	}

	FVector2 operator / (int Value)	const
	{
		FVector2	result;
		result.x = x / static_cast<float>(Value);
		result.y = y / static_cast<float>(Value);
		return result;
	}

	// /=
	const FVector2& operator /= (const FVector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const FVector2& operator /= (float Value)
	{
		x /= Value;
		y /= Value;
		return *this;
	}

	const FVector2& operator /= (double Value)
	{
		x /= static_cast<float>(Value);
		y /= static_cast<float>(Value);
		return *this;
	}

	const FVector2& operator /= (int Value)
	{
		x /= static_cast<float>(Value);
		y /= static_cast<float>(Value);
		return *this;
	}

#pragma endregion Divide

#pragma region Function

	float Length()	const
	{
		return sqrtf(x * x + y * y);
	}

	void Normalize()
	{
		float	Size = Length();

		if (Size == 0.f)
			return;

		x /= Size;
		y /= Size;
	}


	static FVector2 Normalize(const FVector2& v)
	{
		FVector2 result;

		float Size = v.Length();

		if (Size == 0.f)
		{
			return result;
		}

		result.x = v.x / Size;
		result.y = v.y / Size;

		return result;
	}

	float Dot(const FVector2& v) const
	{
		return x * v.x + y * v.y;
	}

	float Distance(const FVector2& v)	const
	{
		FVector2 v1 = *this - v;

		return v1.Length();
	}

	float GetAngle(const FVector2& v)	const
	{
		FVector2	v1 = *this;
		FVector2	v2 = v;

		v1.Normalize();
		v2.Normalize();

		float cosTheta = v1.Dot(v2);

		return DirectX::XMConvertToDegrees(acosf(cosTheta));
	}

	static float GetAngle(const FVector2& v1, const FVector2& v2)
	{
		return 0.f;
	}

#pragma endregion Function
};
