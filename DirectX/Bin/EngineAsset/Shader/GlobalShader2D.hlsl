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

VS_OUTPUT_TEX DefaultTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;

	float3 Pos = input.Pos - cbPivotSize;

	output.Pos = mul(float4(Pos, 1.f), cbWVP);
	output.UV = ComputeAnimation2DUV(input.UV);

	return output;
}

VS_OUTPUT_TEX DefaultTexNoneAnimVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
    
	float3 Pos = input.Pos - cbPivotSize;
    
    // x, y, z 에는 input.Pos의 x, y, z 가 들어가고 w에는 1.f이 들어간다.
    // mul : 행렬 곱셈연산을 해준다.
	output.Pos = mul(float4(Pos, 1.f), cbWVP);
	output.UV = input.UV;

	return output;
}

PS_OUTPUT_COLOR DefaultTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
	float4 TextureColor = tbBaseTexture.Sample(sbPoint, input.UV);
    
	output.Color = TextureColor;
    
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

PS_OUTPUT_COLOR TexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
	output.Color = tbBaseTexture.Sample(sbPoint, input.UV);
    
	return output;
}
