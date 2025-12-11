#include "Share.hlsli"

struct VS_INPUT_COLOR2D
{
	// POSITION = POSITION0
	float3 Pos : POSITION0;
	float4 Color : COLOR0;
};

struct VS_OUTPUT_COLOR2D
{
	// Registers prefixed with SV_ are used exclusively for passing values in the Vertex Shader (VS). 
	// They must not be used elsewhere.

	// system value position
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

// Set entry point name in file option.
VS_OUTPUT_COLOR2D Color2DVS(VS_INPUT_COLOR2D input)
{
	VS_OUTPUT_COLOR2D output; //= (VS_OUTPUT_COLOR2D)0;

	output.Pos = float4(input.Pos, 1.0f);
	output.Color = input.Color;

	return output;
}

struct PS_OUTPUT_COLOR
{
	// print color to 0th back buffer
	float4 Color : SV_TARGET;
};

PS_OUTPUT_COLOR Color2DPS(VS_OUTPUT_COLOR2D input)
{
	PS_OUTPUT_COLOR output;
	output.Color = input.Color;
	return output;
}
