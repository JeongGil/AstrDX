// Write content commonly used in this shader directly.

cbuffer CBTransform : register(b0)
{
	matrix cbWorld;
	matrix sbView;
	matrix cbProj;
	matrix cbWV;
	matrix cbWVP;
	float3 cbPivotSize;
	float cbTransformEmpty;
};

SamplerState sbPoint : register(s0);

Texture2D tbMaskTexture : register(t0);
Texture2D tbTileTexture : register(t1);

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

VS_OUTPUT_TEX BrotatoTileVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
	float3 Pos = input.Pos - cbPivotSize;
	output.Pos = mul(float4(Pos, 1.f), cbWVP);
	output.UV = input.UV;
	return output;
}

struct PS_OUTPUT_COLOR
{
	float4 Color : SV_TARGET;
};

PS_OUTPUT_COLOR BrotatoTilePS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
	float4 MaskColor = tbMaskTexture.Sample(sbPoint, input.UV);
	float4 TileColor = tbTileTexture.Sample(sbPoint, input.UV);
	output.Color.rgb = lerp(MaskColor.rgb, TileColor.rgb, MaskColor.r);
	output.Color.a = 1.0f;
	return output;
}
