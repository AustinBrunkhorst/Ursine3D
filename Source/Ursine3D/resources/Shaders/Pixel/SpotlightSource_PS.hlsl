//depth and color
Texture2D DepthTexture : register(t0);
Texture2D ColorSpecIntTexture: register(t1);
Texture2D NormalTexture: register(t2);
Texture2D SpecPowTexture: register(t3);
Texture2D ShadowmapTexture: register(t4);

//sample type
SamplerState SampleType : register(s0);
SamplerComparisonState PCFSampler : register(s1);

//inv projection 
cbuffer invView : register(b4)
{
    float4x4 invProj;
    float nearPlane;
    float farPlane;
};

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
    float intensity         : packoffset(c0.w);
    float3 lightDirection   : packoffset(c1);
    float innerAngle        : packoffset(c1.w);
    float3 diffuseColor     : packoffset(c2);
    float outerAngle        : packoffset(c2.w);
    float3 falloffValues    : packoffset(c3);
    float lightSize         : packoffset(c3.w);
}

//specular power range
static const float2 g_SpecPowerRange = { 0.1, 250.0 };

/////////////////////////////////////////////////////////////////////
// STRUCTS
//input 
struct DS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 cpPos : TEXCOORD0;
};

//data from the buffers
struct SURFACE_DATA
{
    float LinearDepth;
    float4 Color;
    float3 Normal;
    float SpecInt;
    float SpecPow;
    float Emissive;
};

//material data
struct Material
{
    float3 normal;
    float4 diffuseColor;
    float specIntensity;
    float specPow;
    float emissive;
};

/////////////////////////////////////////////////////////////////////
// FUNCTIONS

float LinearizeDepth(float depth)
{
    return (2.f * nearPlane) / (farPlane + nearPlane - depth * (farPlane - nearPlane));
}

//calculating world position
float3 CalcViewPos(float2 csPos, float linearDepth)
{
    float4 position;
    position.xy = csPos.xy;
    position.z = linearDepth;
    position.w = 1.0;
    position = mul(position, invProj);
    position.xyz /= position.w;
    return position.xyz;
}

float SpotShadowPCF(float3 position)
{
    // Transform the view position to world space
    float4 worldSpace = mul(float4(position, 1.0), invCamView);

    // transform world space into light projected space
    float4 posShadowMap = mul(worldSpace, lightView);
    posShadowMap = mul(posShadowMap, lightProj);

    // Transform the position to shadow clip space
    float3 UVD = posShadowMap.xyz / posShadowMap.w;

    // Convert to shadow map UV values
    UVD.xy = 0.5 * UVD.xy + 0.5;
    UVD.y = 1.0 - UVD.y;
    float offsetX = 1.5;
    float offsetY = 0.00045f;

    float sum = 0;
    float x, y;

    for (y = -1.5; y <= 1.5; y += 1.0)
        for (x = -1.5; x <= 1.5; x += 1.0)
            sum += ShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(x, y) * offsetY, UVD.z);

    return sum / 16.0;

    /*
    return (ShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(-1.5f, 0.5f) * offsetY, UVD.z) +
    ShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(0.5f, 0.5f) * offsetY, UVD.z) +
    ShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(-1.5f, -1.5f) * offsetY, UVD.z) +
    ShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy + float2(0.5, -1.5f) * offsetY, UVD.z)) * 0.25f;*/

    // Compute the hardware PCF value, which compares the depth (z) to the shadowmap.
    //return ShadowmapTexture.SampleCmpLevelZero(PCFSampler, UVD.xy, UVD.z);
}

//unpack the g buffer from our textures
SURFACE_DATA UnpackGBuffer(int2 location)
{
    SURFACE_DATA Out;

    // Cast to 3 component for the load function
    int3 location3 = int3(location, 0);

    // Get the depth value and convert it to linear depth
    float depth = DepthTexture.Load(location3).x;
    Out.LinearDepth = depth;

    // Get the base color and specular intensity
    float4 baseColor = ColorSpecIntTexture.Load(location3);
    Out.Color = baseColor;
    Out.SpecInt;

    // Sample the normal, convert it to the full range and noramalize it
    float4 normalValue = NormalTexture.Load(location3);
    Out.Normal = normalValue.xyz;
    Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

    //grab emissive value
    Out.Emissive = normalValue.w;

    // Scale the specular power back to the original range
    float4 SpecPowerNorm = SpecPowTexture.Load(location3);
    Out.SpecPow = g_SpecPowerRange.x + SpecPowerNorm.x * g_SpecPowerRange.y;
    Out.SpecInt = SpecPowerNorm.w;

    return Out;
}

float3 CalcPoint(float3 position, Material material)
{
    float3 toLight = -lightDirection;
    float3 pixelToCamera = -position;
    float3 lightToPixel = (position - lightPosition);

    // grab the length
    float distanceToPixel = length(lightToPixel);

    // normalize vector from light to pixel
    lightToPixel /= distanceToPixel;

    // get the angle from the ray to the pixel and our main direction
    float angleInCone = (dot(lightToPixel, lightDirection));

    // calculate final attenuation for the angle
    float angleAttenuation = (angleInCone - outerAngle) /
        (innerAngle - outerAngle);

    float distAttenuation = saturate(1.0f - (distanceToPixel / lightSize));

    // Blinn specular
    pixelToCamera = normalize(pixelToCamera);
    float3 HalfWay = normalize(pixelToCamera + toLight);
    float NDotH = saturate(dot(HalfWay, material.normal));
    float specularValue = max(pow(NDotH, material.specPow), 0);

    // diffuse scalar from normal
    float normalScalar = max(dot(material.normal, toLight), 0);

    // calculate final light color
    float3 finalLightColor = (diffuseColor.rgb + specularValue * material.specIntensity);

    // apply dotp, attenuation for distance and angle
    finalLightColor *= material.diffuseColor.xyz * normalScalar * angleAttenuation * distAttenuation * intensity * SpotShadowPCF( position );

    return finalLightColor;
}

float4 main(DS_OUTPUT In) : SV_TARGET
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
    float3 position = CalcViewPos(In.cpPos.xy, gbd.LinearDepth);

    //get the final color, return
    return float4(CalcPoint(position, mat), 1.0f);
}