#ifndef GEOMETRY_COMMON
#define GEOMETRY_COMMON

///////////////////////////////////////////////////////////////////////////////
// CONSTANTS

// specular power range
static const float2 cSpecPowerRange = { 0.1, 250.0 };

///////////////////////////////////////////////////////////////////////////////
// STRUCTURES

// this is where we output to each render target
struct PS_GBUFFER_OUT
{
    float4 ColorSpecInt: SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 SpecPow: SV_TARGET2;
};

///////////////////////////////////////////////////////////////////////////////
// UTILITY FUNCTIONS

// pack the gbuffer structure with information
PS_GBUFFER_OUT PackGBuffer( float4 BaseColor, float3 Normal, float
    SpecIntensity, float SpecPower, float emissive, int objID)
{
    PS_GBUFFER_OUT Out;
    // Normalize the specular power
    float SpecPowerNorm = (SpecPower - cSpecPowerRange.x) / cSpecPowerRange.y;

    // convert id into proper sizes
    int word1 = objID & 0xff;           //first 8 bits
    int word2 = (objID >> 8) & 0xff;    //second 8 bits

    // Pack all the data into the GBuffer structure
    Out.ColorSpecInt = float4(BaseColor);
    Out.Normal = float4(Normal.xyz * 0.5 + 0.5, emissive);
    //                                            first 2 are handle index      last 8 is type
    Out.SpecPow = float4(SpecPowerNorm, word1 /255.f, word2 / 255.f, SpecIntensity);
     
    // return
    return Out;
}

#endif