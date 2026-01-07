// Write content commonly used in Shader

/*
 * Register of shader:
 * Input register: Used to pass vertex information. Used as a combination of NameNumber.
 * Output register: Used to pass completed vertex information. DirectX uses this information to determine the position on the screen and prepare for color output.
 * Constant register: Used to pass general data from C++ code to the shader.
 * Temporary register: A register used during computations.
 */

// A feature that transfers frequently changing values from the CPU to the GPU.
cbuffer CBTransform : register(b0)
{
	matrix cbWorld;
	matrix sbView;
	matrix cbProj;
	matrix cbWV;
	matrix cbWVP;
	float3 cbPivotSize;
	float cbTransformEmpty;
}

cbuffer CBMaterial : register(b1)
{
	float4 cbBaseColor;
	float cbOpacity;
	float3 cbMaterialEmpty;
}

cbuffer CBAnimation2D : register(b2)
{
	float2 cbLTUV;
	float2 cbRBUV;
	int cbEnableAnimation2D;
	int cbAnimation2DTextureType;
	float2 cbAnimation2DEmpty;
}

SamplerState sbPoint : register(s0);
SamplerState sbLinear : register(s1);

float2 ComputeAnimation2DUV(float2 UV)
{
	// cbAnimation2DTextureType: 0 is Sprite animation.
	if (cbEnableAnimation2D == 0 || cbAnimation2DTextureType != 0)
	{
		return UV;
	}

	float2 Result;

	if (UV.x == 0.0f)
	{
		Result.x = cbLTUV.x;
	}
	else
	{
		Result.x = cbRBUV.x;
	}

	if (UV.y == 0.0f)
	{
		Result.y = cbLTUV.y;
	}
	else
	{
		Result.y = cbRBUV.y;
	}

	return Result;
}
