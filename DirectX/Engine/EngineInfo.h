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
#include <charconv>
#include <array>
#include <atlbase.h>
#include <atlconv.h>

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

#define GRAVITY2D (980.f)

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
	FVertexColor(FVertexColor&& other) noexcept = default;
	FVertexColor& operator=(const FVertexColor& other) = default;
	FVertexColor& operator=(FVertexColor&& other) noexcept = default;
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
	FVertexTex(FVertexTex&& other) noexcept = default;
	FVertexTex& operator=(const FVertexTex& other) = default;
	FVertexTex& operator=(FVertexTex&& other) noexcept = default;
};

struct FTextureFrame
{
	FTextureFrame() = default;

	FTextureFrame(const FVector2& start, const FVector2& size)
		: Start(start),
		Size(size)
	{
	}

	FTextureFrame(FVector2&& start, FVector2&& size)
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
	FTextureFrame(FTextureFrame&& other) noexcept = default;
	FTextureFrame& operator=(const FTextureFrame& other) = default;
	FTextureFrame& operator=(FTextureFrame&& other) noexcept = default;

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
		Overlap,
		Block,
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

struct TableID
{
	int value = -1;

	explicit TableID(int v) : value(v) {}
	explicit TableID(unsigned int v) : value(static_cast<int>(v)) {}

	operator int() const
	{
		return value;
	}

	bool operator==(const TableID& other) const { return value == other.value; }
	bool operator!=(const TableID& other) const { return value != other.value; }
	bool operator<(const TableID& other) const { return value < other.value; }

	TableID(const TableID& other) = default;
	TableID(TableID&& other) noexcept = default;
	TableID& operator=(const TableID& other) = default;
	TableID& operator=(TableID&& other) noexcept = default;
};

namespace std
{
	template <>
	struct hash<TableID>
	{
		size_t operator()(const TableID& t) const noexcept
		{
			return hash<unsigned int>{}(t.value);
		}
	};
}
