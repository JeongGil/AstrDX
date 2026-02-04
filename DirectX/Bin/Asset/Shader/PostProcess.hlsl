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

struct VS_OUTPUT_TEX
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct PS_OUTPUT_COLOR
{
    float4 Color : SV_TARGET;
};

Texture2D tbBaseTexture : register(t0);
Texture2D tbHitTexture : register(t1);

SamplerState sbPoint : register(s0);

VS_OUTPUT_TEX HitVS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
    
    output.Pos = NullPos[VertexID];
    output.UV = NullUV[VertexID];
    
    return output;
}

PS_OUTPUT_COLOR HitPS(VS_OUTPUT_TEX input)
{
    PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
    output.Color = tbBaseTexture.Sample(sbPoint, input.UV);
    
    return output;
}
