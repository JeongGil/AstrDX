#pragma once

#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <memory>
#include <string>
#include <functional>
#include <typeindex>
#include <random>

// check memory leak
#include <crtdbg.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "resource.h"

#include "FMatrix.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#define SAFE_DELETE(p) { if(p) { delete (p); (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(P) { if (p) {delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }

struct FResolution
{
	int Width = 0;
	int Height = 0;
};

struct FVertexColor
{
	FVector Pos;
	FColor Color;

	FVertexColor() = default;

	FVertexColor(float x, float y, float z, float r, float g, float b, float a)
		: Pos(x, y, z), Color(r, g, b, a)
	{
	}

	FVertexColor(FVector pos, FColor color)
		: Pos(std::move(pos)),
		Color(std::move(color))
	{
	}

	FVertexColor(const FVertexColor& other) = default;

	FVertexColor(FVertexColor&& other) noexcept
		: Pos(std::move(other.Pos)),
		Color(std::move(other.Color))
	{
	}

	FVertexColor& operator=(const FVertexColor& other)
	{
		if (this == &other)
			return *this;
		Pos = other.Pos;
		Color = other.Color;
		return *this;
	}

	FVertexColor& operator=(FVertexColor&& other) noexcept
	{
		if (this == &other)
			return *this;
		Pos = std::move(other.Pos);
		Color = std::move(other.Color);
		return *this;
	}
};

struct FVertexTex
{
	FVector Pos;
	FVector2 UV;

	FVertexTex() = default;

	FVertexTex(float x, float y, float z, float u, float v)
		: Pos(x, y, z),
		UV(u, v)
	{
	}

	FVertexTex(const FVertexTex& other) = default;

	FVertexTex(FVertexTex&& other) noexcept
		: Pos(std::move(other.Pos)),
		UV(std::move(other.UV))
	{
	}

	FVertexTex& operator=(const FVertexTex& other)
	{
		if (this == &other)
			return *this;
		Pos = other.Pos;
		UV = other.UV;
		return *this;
	}

	FVertexTex& operator=(FVertexTex&& other) noexcept
	{
		if (this == &other)
			return *this;
		Pos = std::move(other.Pos);
		UV = std::move(other.UV);
		return *this;
	}
};

struct FTextureFrame
{
	FTextureFrame() = default;

	FTextureFrame(FVector2& start, FVector2& size)
		: Start(std::move(start)),
		Size(std::move(size))
	{
	}

	FTextureFrame(float x, float y, float w, float h)
		: Start(x, y),
		Size(w, h)
	{
	}

	FTextureFrame(const FTextureFrame& other) = default;

	FTextureFrame(FTextureFrame&& other) noexcept
		: Start(std::move(other.Start)),
		Size(std::move(other.Size))
	{
	}

	FTextureFrame& operator=(const FTextureFrame& other)
	{
		if (this == &other)
			return *this;
		Start = other.Start;
		Size = other.Size;
		return *this;
	}

	FTextureFrame& operator=(FTextureFrame&& other) noexcept
	{
		if (this == &other)
			return *this;
		Start = std::move(other.Start);
		Size = std::move(other.Size);
		return *this;
	}

	FVector2 Start;
	FVector2 Size;
};

enum class EAnimation2DTextureType
{
	None = -1,
	SpriteSheet,
	Frame,
};

enum class EAssetType
{
	None = -1,
	Mesh,
	Shader,
	ConstantBuffer,
	Material,
	Texture,
	Animation2D,
};

enum class EColliderType
{
	Box2D,
	Sphere2D,
	Line2D,
};

struct FBox2DInfo
{
	FVector Center;
	FVector Axis[2] =
	{
	FVector::Axis[EAxis::X],
	FVector::Axis[EAxis::Y],
	};

	FVector2 HalfExtent = FVector2(1.f, 1.f);
};

namespace ECollisionChannel
{
	enum Type
	{
		Static,
		Player,
		Monster,
		Custom1,
		Custom2,
		Custom3,
		Custom4,
		Custom5,
		Custom6,
		Custom7,
		Custom8,
		Custom9,
		Custom10,
		End,
	};
}

namespace ECollisionInteraction
{
	enum Type
	{
		Ignore,
		Collision,
		End,
	};
}

struct FCollisionChannel
{
	std::string Name;
	ECollisionChannel::Type Channel = ECollisionChannel::Static;
};

struct FCollisionProfile
{
	std::string Name;
	FCollisionChannel* Channel;
	bool bEnable;
	ECollisionInteraction::Type Interaction[ECollisionInteraction::End] = {};
};

struct FSphere2DInfo
{
	FVector Center;
	float Radius = 0.f;
};

struct FLine2DInfo
{
	FVector Start;
	FVector End;
};
