#include "../Headers/Randomness_H.hlsl"
#include "../Headers/LightingCommon_H.hlsl"

//buffer for light data
cbuffer PointLightBuffer : register(b3)
{
    float3  lightPos        : packoffset(c0);
    float   radius          : packoffset(c0.w);
    float3  diffuseColor    : packoffset(c1);
    float   intensity       : packoffset(c1.w);
}

//specular power range

/////////////////////////////////////////////////////////////////////
// STRUCTS
//input 
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 cpPos    : TEXCOORD0;
};

/////////////////////////////////////////////////////////////////////
// FUNCTIONS

float3 CalcPoint( float3 position, Material material )
{
    float3 toLight = lightPos.xyz - position;
    float3 toEye = normalize(-position);
    float DistToLight = length(toLight);
    toLight /= DistToLight;

    // Phong diffuse
    float NDotL;
    float3 diffuse = CalcDiffuseValue(toLight, material.normal, diffuseColor.rgb, NDotL); 

    // Blinn specular
    float3 HalfWay = normalize(toEye + toLight);
    float specularValue = CalcSpecularValue(HalfWay, material.normal, material);

    // Attenuation
    float Attn = saturate(1.0f - (DistToLight / radius));

    // calculate final color
    return CalculateLightmapResponse(toLight, -toLight, toEye, material) * Attn;
    // return CalcFinalLightValue(Attn * NDotL * intensity, diffuse, specularValue, material);
}

/////////////////////////////////////////////////////////////////////
// MAIN
float4 main(PS_INPUT In ) : SV_TARGET
{
    // Unpack the GBuffer
    SURFACE_DATA gbd = UnpackGBuffer(In.Position.xy);

    // Convert the data into the material structure
    Material mat;
    mat.normal = gbd.Normal;
    mat.diffuseColor = gbd.Color;
    mat.specPow = gbd.SpecPow; 
    mat.specIntensity = gbd.SpecInt;
    mat.emissive = gbd.Emissive;

    //calculate world position
    In.cpPos.xy /= In.cpPos.w;
    float3 pos = CalcViewPos(In.cpPos.xy, gbd.LinearDepth);

    //get the final color
    float4 finalColor;
    finalColor.xyz = CalcPoint(pos, mat);
    finalColor.w = 1.0; 

    return float4(finalColor.xyz, 1.0f);
}