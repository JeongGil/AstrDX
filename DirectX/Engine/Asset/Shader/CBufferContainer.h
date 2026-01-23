#pragma once

#include "../../FMatrix.h"

namespace EShaderBufferType
{
	enum Type : unsigned int
	{
		None		= 0,
		Vertex		= 1 << 0,
		Pixel		= 1 << 1,
		Domain		= 1 << 2,
		Hull		= 1 << 3,
		Geometry	= 1 << 4,
		Compute		= 1 << 5,
	};

	static constexpr unsigned int VP = Vertex | Pixel;
	static constexpr unsigned int GRAPHIC = Vertex | Pixel | Domain | Hull | Geometry;
	static constexpr unsigned int ALL = GRAPHIC | Compute;
}

// It must be created to match the constant buffers declared in the shader.
struct FCBufferTransformData
{
	FMatrix World;
	FMatrix View;
	FMatrix Proj;
	FMatrix WV;
	FMatrix WVP;
	FVector PivotSize;
	float Empty;
};

struct FCBufferMaterialData
{
	FVector4 BaseColor;
	float Opacity;
	FVector Empty;
};

struct FCBufferAnimation2DData
{
	FVector2 LTUV;
	FVector2 RBUV;
	int bEnableAnimation2D = 0;
	int TextureType = -1;
	int TextureSymmetry = 0;
	float Empty;
};

struct FCBufferColliderData
{
	FColor Color;
};

struct FCBufferUIDefaultData
{
	FVector4 BrushTint;
	FVector4 WidgetColor;
	FVector2 BrushLTUV;
	FVector2 BrushRBUV;
	int BrushAnimEnable;
	int BrushTextureEnable;
	FVector2 BrushEmpty;
};
