#pragma once

#include <algorithm>
#include <array>
#include <atlbase.h>
#include <atlconv.h>
#include <charconv>
#include <chrono>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <process.h>
#include <random>
#include <ranges>
#include <stack>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <Windows.h>

// check memory leak
#include <crtdbg.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <d2d1.h>
#include <dwrite_3.h>

#include "resource.h"

#include "FMatrix.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#define SAFE_DELETE(p) { if(p) { delete (p); (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(P) { if (p) {delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }

#define GRAVITY2D (980.f)

struct FResolution
{
	int Width = 0;
	int Height = 0;
};

struct FVertexBuffer
{
	ID3D11Buffer* Buffer = nullptr;

	// size of single vertex
	int Size = 0;
	int Count = 0;

	FVertexBuffer() = default;
	~FVertexBuffer()
	{
		if (Buffer)
		{
			Buffer->Release();
		}
	}
};

struct FIndexBuffer
{
	ID3D11Buffer* Buffer = nullptr;

	// size of single index
	int Size = 0;
	int Count = 0;

	// used to specify the index type when accessing or outputting using an index.
	DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;

	FIndexBuffer() = default;
	~FIndexBuffer()
	{
		if (Buffer)
		{
			Buffer->Release();
		}
	}
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
	Array,
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
	Sound,
	Font,
	FontCollection,
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

struct FCollisionManifold
{
	FVector ContactPoint;
	FVector Normal;
	float Penetration = 0.f;
};

namespace EMouseState
{
	enum Type
	{
		Normal,
		State1,
		State2,
		State3,
		State4,
		State5,
		State6,
		State7,
		State8,
		State9,
		State10,
		End
	};
}

enum ETileShape
{
	Rect,
	Isometric,
};

namespace ETileTextureType
{
	enum Type
	{
		Back,
		Tile,
		End
	};
}

struct FTileFrame
{
	FVector2 Start;
	FVector2 End;
};

struct FInstancingData
{
	FVector4 WVP0;
	FVector4 WVP1;
	FVector4 WVP2;
	FVector4 WVP3;
	FVector2 LTUV;
	FVector2 RBUV;
	FColor	BaseColor;
	FVector PivotSize;
	int ArrayTextureEnable = 0;
	int AnimFrame = 0;
};

struct FRenderKey
{
	size_t MeshID;
	size_t ShaderID;
	size_t TextureID;

	friend bool operator==(const FRenderKey& Lhs, const FRenderKey& Rhs)
	{
		return Lhs.MeshID == Rhs.MeshID
			&& Lhs.ShaderID == Rhs.ShaderID
			&& Lhs.TextureID == Rhs.TextureID;
	}

	friend bool operator!=(const FRenderKey& Lhs, const FRenderKey& Rhs)
	{
		return !(Lhs == Rhs);
	}
};

struct FRenderKeyHash
{
	size_t operator()(const FRenderKey& Key) const
	{
		size_t Seed = Key.MeshID;
		Seed ^= Key.TextureID + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
		Seed ^= Key.ShaderID + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);

		return Seed;
	}
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
