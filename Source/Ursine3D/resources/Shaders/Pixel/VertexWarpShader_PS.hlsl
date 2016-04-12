#include "../Headers/GeometryCommon_H.hlsl"

// texture
Texture2D gShaderTexture    : register(t0);
Texture2D gNormalMap        : register(t1);

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
    float4 tangent      : TANGENT;
    float4 bitangent    : BITANGENT;
    float2 uv           : UV;
    float2 emisiveTex          : EM_UV;
};

PS_GBUFFER_OUT main(PS_INPUT input)
{
    float4 baseColor = float4(gShaderTexture.Sample(SampleType, input.uv).xyz * color.xyz, color.a);
    float3 normal = input.normal.xyz;

    // sample
    float3 normalValue = (gNormalMap.Sample(SampleType, input.emisiveTex).xyz);

    float3x3 texSpace = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz);

    // calculate final normal based upon bitan tan and normal
    float3 finalNormal = normalize(mul(normalValue, texSpace));

    PS_GBUFFER_OUT buff = PackGBuffer(baseColor, normal, specularIntensity, specularPower, emissive * normalValue.x, objID);


    return buff;
}