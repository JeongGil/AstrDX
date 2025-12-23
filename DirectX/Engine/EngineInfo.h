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
	FVector3 Pos;
	FVector4 Color;

	FVertexColor() = default;

	FVertexColor(float x, float y, float z, float r, float g, float b, float a)
		: Pos(x, y, z), Color(r, g, b, a)
	{
	}

	FVertexColor(FVector3 pos, FVector4 color)
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
	FVector3 Pos;
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
