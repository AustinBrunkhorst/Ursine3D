#include "../Headers/LightingCommon_H.hlsl"

// shadowmap
Texture2D gShadowmapTexture: register(t4);

//sample type
SamplerComparisonState PCFSampler : register(s1);

cbuffer ShadowProj : register(b13)
{
    float4x4 invCamView;
    matrix lightView;
    matrix lightProj;
};

//buffer for light data
cbuffer SpotLightBuffer : register(b11)
{
    float3 lightPosition    : packoffset(c0);
    float intensity : packoffset(c0.w);
    float3 lightDirection   : packoffset(c1);
    float innerAngle : packoffset(c1.w);
    float3 diffuseColor     : packoffset(c2);
    float outerAngle : packoffset(c2.w);
    float3 falloffValues    : packoffset(c3);
    float lightSize : packoffset(c3.w);
}

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

float SpotShadowPCF(float3 position)
{
    // Transform the view position to world space
    float4 worldSpace = mul(
        float4(
            position,
            1.0
            ),
        invCamView
        );

    // transform world space into light projected space
    float4 posShadowMap = mul(
        worldSpace,
        lightView
        );

    posShadowMap = mul(
        posShadowMap,
        lightProj
        );

    // Transform the position to shadow clip space
    float3 UVD = posShadowMap.xyz / posShadowMap.w;

    // Convert to shadow map UV values
    UVD.xy = 0.5 * UVD.xy + 0.5;
    UVD.y = 1.0 - UVD.y;
    //float offsetX = 1.5;
    //float offsetY = 0.00045f;

    //float sum = 0;
    //float x, y;

    //for (y = -1.5; y <= 1.5; y += 1.5)
    //    for (x = -1.5; x <= 1.5; x += 1.5)
    //        sum += 

    return saturate(gShadowmapTexture.SampleCmpLevelZero(
        PCFSampler,
        UVD.xy,
        UVD.z
        ));

    /*
    return (gShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(-1.5f, 0.5f) * offsetY, UVD.z) +
    gShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(0.5f, 0.5f) * offsetY, UVD.z) +
    gShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(-1.5f, -1.5f) * offsetY, UVD.z) +
    gShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(0.5, -1.5f) * offsetY, UVD.z)) * 0.25f;*/

    // Compute the hardware PCF value, which compares the depth (z) to the shadowmap.
    //return gShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy, UVD.z);
}

float3 CalcPoint(float3 position, Material material)
{
    float3 toLight = normalize(lightPosition - position);
    float3 toEye = normalize(-position);
    float3 lightToPixel = position - lightPosition;

    // grab the length
    float distanceToPixel = length(lightToPixel);
    lightToPixel /= distanceToPixel;

    // Phong diffuse
    float NDotL;
    float3 diffuse = CalcDiffuseValue(toLight, material.normal, diffuseColor.rgb, NDotL);

    // Blinn specular
    float3 HalfWay = normalize(toEye + toLight);
    float specularValue = CalcSpecularValue(HalfWay, material.normal, material);

    // angle attenuation
    float angleInCone = dot(lightToPixel, lightDirection);
    float angleAttenuation = saturate((angleInCone - outerAngle) / (innerAngle - outerAngle));

    // distance attenuation
    float Attn = saturate(1.0f - (distanceToPixel / lightSize));

    return CalculateLightmapResponse(toLight, -toLight, toEye, material) * Attn * angleAttenuation * diffuseColor.xyz  * SpotShadowPCF(position) * intensity;
    // return CalcFinalLightValue(angleAttenuation * Attn * NDotL * intensity * SpotShadowPCF(position), diffuse, specularValue, material);
}

float4 main(PS_INPUT In) : SV_TARGET
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

//calculate view position
In.cpPos.xy /= In.cpPos.w;
float3 position = CalcViewPos(
    In.cpPos.xy,
    gbd.LinearDepth
    );

//get the final color, return
return float4(CalcPoint(position, mat), 1.0f);
}