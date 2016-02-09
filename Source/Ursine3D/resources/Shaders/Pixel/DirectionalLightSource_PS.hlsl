//depth and color
Texture2D DepthTexture : register(t0);
Texture2D ColorSpecIntTexture: register(t1);
Texture2D NormalTexture: register(t2);
Texture2D SpecPowTexture: register(t3);

//sample type
SamplerState SampleType : register(s0);

//buffer for light data
cbuffer DirectionalLightBuffer : register(b2)
{
    float4 lightDirection   : packoffset(c0);
    float3 diffuseColor     : packoffset(c1);
    float intensity         : packoffset(c1.w);
    float4 lightPosition    : packoffset(c2);
}

cbuffer InvProj : register(b4)
{
    float4x4 InvProj;
    float nearPlane;
    float farPlane;
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
//converting depth
//near is 0.1, far is 100, this needs to be modified
float ConvertDepthToLinear( float depth )
{
    float f = farPlane;
    float n = nearPlane;
    float z = (2 * n) / (f + n - depth * (f - n));
    return z;
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
    float4 baseColor = ColorSpecIntTexture.Load( location3 );
    Out.Color = baseColor;
    Out.SpecInt;

    // Sample the normal, convert it to the full range and noramalize it
    float4 normalValue = NormalTexture.Load(location3);
    Out.Normal = normalValue.xyz;
    Out.Normal = normalize( Out.Normal * 2.0 - 1.0 );

    //grab emissive value
    Out.Emissive = normalValue.w;

    // Scale the specular power back to the original range
    float4 SpecPowerNorm = SpecPowTexture.Load(location3);
    Out.SpecPow = g_SpecPowerRange.x + SpecPowerNorm.x * g_SpecPowerRange.y;
    Out.SpecInt = SpecPowerNorm.w;

    return Out;
}

float3 CalcPoint( float3 position, Material material )
{
    //float3 ToLight = -lightDirection.xyz;
    //float3 ToEye = -position;

    //// Phong diffuse
    //float NDotL = saturate( dot( ToLight, material.normal ) );
    //float3 finalColor = diffuseColor.rgb * (intensity) * material.diffuseColor.xyz;

    //// Blinn specular
    //ToEye = normalize( ToEye );
    //float3 HalfWay = normalize( ToEye + ToLight );
    //float NDotH = saturate( dot( HalfWay, material.normal ) );
    //finalColor += diffuseColor.rgb * max( pow( NDotH, material.specPow ), 0 ) * material.specIntensity;

    //finalColor *= NDotL;

    //return finalColor * (1.f - material.emissive) + material.diffuseColor.xyz * material.emissive;

    ///////////////////////////


    float3 toLight = -lightDirection.xyz;
    float3 pixelToCamera = -position;

    // Blinn specular
    pixelToCamera = normalize(pixelToCamera);
    float3 HalfWay = normalize(pixelToCamera + toLight);
    float NDotH = saturate(dot(HalfWay, material.normal));
    float specularValue = saturate(pow(NDotH, material.specPow));

    // diffuse scalar from normal
    float normalScalar = saturate(dot(material.normal, toLight));

    // calculate final light color
    float3 finalLightColor = (diffuseColor.rgb + specularValue * material.specIntensity);

    // apply normal scalar
    finalLightColor *= normalScalar;

    return finalLightColor;
}


float4 main( DS_OUTPUT In ) : SV_TARGET
{
    // Unpack the GBuffer
    SURFACE_DATA gbd = UnpackGBuffer( In.Position.xy );

    // Convert the data into the material structure
    Material mat;
    mat.normal = gbd.Normal;
    mat.diffuseColor = gbd.Color;
    mat.specPow = gbd.SpecPow;
    mat.specIntensity = gbd.SpecInt;
    mat.emissive = gbd.Emissive;

    //calculate world position
    In.cpPos.xy /= In.cpPos.w;
    float3 pos = CalcWorldPos( In.cpPos.xy, gbd.LinearDepth );

    //get the final color
    float4 finalColor;
    finalColor.xyz = CalcPoint( pos, mat );
    finalColor.w = 1.0;

    return finalColor;
}