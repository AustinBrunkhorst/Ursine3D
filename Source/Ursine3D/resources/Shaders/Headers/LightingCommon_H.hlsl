#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

///////////////////////////////////////////////////////////////////////////////
// CONSTANTS

static const float2 cSpecPowerRange = { 0.1, 250.0 };

///////////////////////////////////////////////////////////////////////////////
// RESOURCES

// input textures
Texture2D    gDepthTexture          : register(t0);
Texture2D    gColorSpecIntTexture   : register(t1);
Texture2D    gNormalTexture         : register(t2);
Texture2D    gSpecPowTexture        : register(t3);
Texture2D    gLightResponse         : register(t5);

// sample state
SamplerState SampleType             : register(s0);

//inv projection 
cbuffer InverseProjection           : register(b4)
{
    float4x4    gInvProj;
    float       gAValue;
    float       gBValue;
};

// cel shading data
cbuffer ShadowFalloff               : register(b12)
{
    float gLightStep;
    float gBorderCutoff;
    float2 buffer;
}

///////////////////////////////////////////////////////////////////////////////
// STRUCTURES
//data from the buffers
struct SURFACE_DATA
{
    float   LinearDepth;
    float4  Color;
    float3  Normal;
    float   SpecInt;
    float   SpecPow;
    float   Emissive;
};

//material data
struct Material
{
    float3  normal;
    float4  diffuseColor;
    float   specIntensity;
    float   specPow;
    float   emissive;
};

///////////////////////////////////////////////////////////////////////////////
// UTILITY FUNCTIONS

// convert given depth value from exponential to linear
float LinearizeDepth(float depth)
{
    return gBValue / (depth - gAValue);
}

// calculate view-space position, given depth
float3 CalcViewPos(float2 csPos, float linearDepth)
{
    float4 position;
    position.xy = csPos.xy;
    position.z = linearDepth;
    position.w = 1.0f;
    position = mul( position, gInvProj );
    position.xyz /= position.w;
    return position.xyz;
}

//unpack a g-buffer into data
SURFACE_DATA UnpackGBuffer(int2 location)
{
    SURFACE_DATA Out;

    // Cast to 3 component for the load function
    int3 location3 = int3(location, 0);

    // Get the depth value and convert it to linear depth
    float depth = gDepthTexture.Load( location3 ).x;
    Out.LinearDepth = depth;

    // Get the base color and specular intensity
    float4 baseColor = gColorSpecIntTexture.Load( location3 );
    Out.Color = baseColor;

    // Sample the normal, convert it to the full range and noramalize
    float4 normalValue = gNormalTexture.Load(location3);
    Out.Normal = normalValue.xyz;
    Out.Normal = normalize( Out.Normal * 2.0 - 1.0 );

    //grab emissive value
    Out.Emissive = normalValue.w;

    // Scale the specular power back to the original range
    float4 SpecPowerNorm = gSpecPowTexture.Load(location3);
    Out.SpecPow = cSpecPowerRange.x + SpecPowerNorm.x * cSpecPowerRange.y;
    Out.SpecInt = SpecPowerNorm.w;

    return Out;
}

///////////////////////////////////////////////////////////////////////////////
// LIGHT FUNCTIONS

// calculate diffuse value of light
float3 CalcDiffuseValue(float3 lightDir, float3 normal, float3 diffuse, out float dotp)
{
    dotp = dot(lightDir, normal);
    return diffuse;
}

// calculate specular response of material
float CalcSpecularValue(float3 halfwayVector, float3 normal, Material material)
{
    float NDotH = saturate(dot(halfwayVector, normal));
    return max( pow(NDotH, material.specPow), 0.0f ) * material.specIntensity;
}

// calculate cel-shaded diffuse value of light
float3 CalcCelDiffuseValue(float3 lightDir, float3 normal, float3 diffuse, out float dotp)
{
    dotp = dot(lightDir, normal);
    dotp = (float)((int)(dotp * gLightStep)) / gLightStep;
    return diffuse;
}

// calculate cel-shaded specular response of material
float CalcCelSpecularValue(float3 halfwayVector, float3 normal, Material material)
{
    float NDotH = saturate(dot(halfwayVector, normal));
    return (float)((int)(max(pow(NDotH, material.specPow), 0.0f) * material.specIntensity * gLightStep)) / gLightStep;
}

// calculate a final light value, given some overall scalar, 
    // the diffuse and specular, and the material
float3 CalcFinalLightValue(float scalar, float3 diffuse, float specularValue, Material material)
{
    return scalar * ((diffuse * material.diffuseColor.rgb) + specularValue);
}

// calculate light response from lightmap
float3 CalculateLightmapResponse(float3 toLight, float3 lightDirection, float3 toEye, Material material)
{
    float2 coordinate;

    // calculate x coord
    coordinate.x = saturate(dot(material.normal, toLight));

    // calculate y coord
    float NDotH = saturate(dot(toEye, normalize(reflect(lightDirection, material.normal))));
    coordinate.y = 1.0f - pow(saturate(NDotH), material.specPow) * material.specIntensity;

    return gLightResponse.Sample(SampleType, coordinate.xy).xyz * material.diffuseColor.xyz;
}

#endif