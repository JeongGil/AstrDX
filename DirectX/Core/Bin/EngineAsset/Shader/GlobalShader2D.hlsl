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

// Texture. use register 0.
Texture2D tbBaseTexture : register(t0);

// Set entry point name in file option.
VS_OUTPUT_COLOR2D Color2DVS(VS_INPUT_COLOR2D input)
{
	VS_OUTPUT_COLOR2D output = (VS_OUTPUT_COLOR2D) 0;

	float3 Pos = input.Pos - cbPivotSize;

	output.Pos = mul(float4(Pos, 1.f), cbWVP);
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
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
	output.Color = input.Color;
	return output;
}

PS_OUTPUT_COLOR MaterialColor2DPS(VS_OUTPUT_COLOR2D input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
	output.Color = cbBaseColor;
	output.Color.a = cbOpacity;

	return output;
}

struct VS_INPUT_TEX
{
	float3 Pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VS_OUTPUT_TEX
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

VS_OUTPUT_TEX DefaultTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

	float3 Pos = input.Pos - cbPivotSize;

	output.Pos = mul(float4(Pos, 1.f), cbWVP);
	output.UV = ComputeAnimation2DUV(input.UV);

	return output;
}

PS_OUTPUT_COLOR MaterialTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;

	float4 TextureColor = tbBaseTexture.Sample(sbPoint, input.UV);

	output.Color.rgb = TextureColor.rgb * cbBaseColor.rgb;
	output.Color.a = TextureColor.a * cbOpacity;

	return output;
}

float4 FrameVS(float3 Pos : POSITION0) : SV_POSITION
{
	float4 OutputPos = mul(float4(Pos, 1.f), cbWVP);    
	return OutputPos;
}

static float4 NullPos[4] =
{
	float4(-1.f, 1.f, 0.f, 1.f),
    float4(1.f, 1.f, 0.f, 1.f),
    float4(-1.f, -1.f, 0.f, 1.f),
    float4(1.f, -1.f, 0.f, 1.f)
};

static float2 NullUV[4] =
{
	float2(0.f, 0.f),
    float2(1.f, 0.f),
    float2(0.f, 1.f),
    float2(1.f, 1.f)
};

VS_OUTPUT_TEX NullTexVS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
    
	output.Pos = NullPos[VertexID];
	output.UV = NullUV[VertexID];
    
	return output;
}

PS_OUTPUT_COLOR TexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
	output.Color = tbBaseTexture.Sample(sbPoint, input.UV);
    
	return output;
}
