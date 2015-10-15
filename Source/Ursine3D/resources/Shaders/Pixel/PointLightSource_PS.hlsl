//depth and color
Texture2D DepthTexture : register(t0);
Texture2D ColorSpecIntTexture: register(t1);
Texture2D NormalTexture: register(t2);
Texture2D SpecPowTexture: register(t3);

//buffer for light data
cbuffer PointLightBuffer : register(b3)
{
    float3 lightPos     : packoffset(c0);
    float radius        : packoffset(c0.w);
    float3 diffuseColor : packoffset(c1);
    float intensity     : packoffset(c1.w);
}

cbuffer invProj : register(b4)
{
    matrix InvProj;
};

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
    float3 Color;
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
//converting depth
//near is 0.1, far is 100, this needs to be modified
float ConvertDepthToLinear( float depth )
{
    float f = 100.0;
    float n = 0.1;
    float z = (2 * n) / (f + n - depth * (f - n));
    return depth;
}

//calculating world position
float3 CalcWorldPos( float2 csPos, float linearDepth )
{
    float4 position;
    position.xy = csPos.xy;
    position.z = linearDepth;
    position.w = 1.0;
    position = mul( position, InvProj );
    position.xyz /= position.w;
    return position.xyz;
}

//unpack the g buffer from our textures
SURFACE_DATA UnpackGBuffer( int2 location )
{
    SURFACE_DATA Out;

    // Cast to 3 component for the load function
    int3 location3 = int3(location, 0);

    // Get the depth value and convert it to linear depth
    float depth = DepthTexture.Load( location3 ).x;
    Out.LinearDepth = ConvertDepthToLinear( depth );

    // Get the base color and specular intensity
    float4 baseColorSpecInt = ColorSpecIntTexture.Load( location3 );
    Out.Color = baseColorSpecInt.xyz;
    Out.SpecInt = baseColorSpecInt.w;

    // Sample the normal, convert it to the full range and noramalize it
    Out.Normal = NormalTexture.Load( location3 ).xyz;
    Out.Normal = normalize( Out.Normal * 2.0 - 1.0 );

    // Scale the specular power back to the original range
    float2 SpecPowerNorm = SpecPowTexture.Load( location3 ).xy;
    Out.SpecPow = g_SpecPowerRange.x + SpecPowerNorm.x * g_SpecPowerRange.y;

    //grab emissive value
    Out.Emissive = SpecPowerNorm.y;

    return Out;
}

float3 CalcPoint( float3 position, Material material )
{
    float3 ToLight = lightPos.xyz - position;
    float3 ToEye = -position;
    float DistToLight = length( ToLight );

    // Phong diffuse
    ToLight /= DistToLight; // Normalize
    float NDotL = saturate( dot( ToLight, material.normal ) );
    float3 finalColor = diffuseColor.rgb * NDotL * (intensity);

    // Attenuation
    float attenuation = saturate( 1.0f - (DistToLight / radius) );
    finalColor *= material.diffuseColor * attenuation;

    // Blinn specular
    ToEye = normalize( ToEye );
    float3 HalfWay = normalize( ToEye + ToLight );
    float NDotH = saturate( dot( HalfWay, material.normal ) );
    finalColor += diffuseColor.rgb * max( pow( NDotH, material.specPow ), 0) * material.specIntensity * attenuation * NDotL;

    return finalColor;
}

/////////////////////////////////////////////////////////////////////
// MAIN
float4 main( DS_OUTPUT In ) : SV_TARGET
{
    // Unpack the GBuffer
    SURFACE_DATA gbd = UnpackGBuffer( In.Position.xy );

    // Convert the data into the material structure
    Material mat;
    mat.normal = gbd.Normal;
    mat.diffuseColor.xyz = gbd.Color;
    mat.diffuseColor.w = 1.0; // Fully opaque
    mat.specPow = gbd.SpecPow;
    mat.specIntensity = gbd.SpecInt;
    mat.emissive = gbd.Emissive;

    In.cpPos.xy /= In.cpPos.w;
    float3 pos = CalcWorldPos( In.cpPos.xy, gbd.LinearDepth );

    float4 finalColor;
    finalColor.xyz = CalcPoint( pos, mat );
    finalColor.w = 1.0;

    return finalColor;
}