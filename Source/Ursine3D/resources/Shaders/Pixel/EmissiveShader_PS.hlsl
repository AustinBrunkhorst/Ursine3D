//depth and color
Texture2D gDepthTexture : register(t0);
Texture2D gColorSpecIntTexture: register(t1);
Texture2D gNormalTexture: register(t2);
Texture2D gSpecPowTexture: register(t3);

//sample type
SamplerState SampleType : register(s0);

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
    float4 Color;
    float Emissive;
};

/////////////////////////////////////////////////////////////////////
// FUNCTIONS

//unpack the g buffer from our textures
SURFACE_DATA UnpackGBuffer( int2 location )
{
    SURFACE_DATA Out;
    int3 location3 = int3(location, 0);

    float4 baseColor = gColorSpecIntTexture.Load(location3);
    Out.Color = baseColor;

    // Sample the normal, convert it to the full range and noramalize it
    float4 normalValue = gNormalTexture.Load(location3);

    //grab emissive value
    Out.Emissive = normalValue.w;

    return Out;
}

float4 main( DS_OUTPUT In ) : SV_TARGET
{
    // Unpack the GBuffer
    SURFACE_DATA gbd = UnpackGBuffer( In.Position.xy );

    return gbd.Emissive * gbd.Color;
}