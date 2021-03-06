#include "../Headers/GeometryCommon_H.hlsl"

// texture
Texture2D gShaderTexture    : register(t0);

// sample type
SamplerState SampleType     : register(s0);

cbuffer MaterialBuffer      : register(b10)
{
    float emissive;
    float specularPower;
    float specularIntensity;
    int objID;
};

cbuffer PrimColorBuffer : register(b5)
{
    float4 color;
}

struct PS_INPUT
{
    float4 Pos          : SV_POSITION;
    float4 normal       : NORMAL;
    float2 uv           : UV;
};

PS_GBUFFER_OUT main( PS_INPUT input )
{
    float4 baseColor = float4(gShaderTexture.Sample(SampleType, input.uv).xyz * color.xyz, color.a);
    float3 normal = input.normal.xyz;

    PS_GBUFFER_OUT buff = PackGBuffer( baseColor, normal, specularIntensity, specularPower, emissive, objID );


    return buff;
}