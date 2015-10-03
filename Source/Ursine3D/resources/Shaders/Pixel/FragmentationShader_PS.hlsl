//texture
Texture2D shaderTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);

cbuffer MaterialBuffer : register(b10)
{
    float emissive;
    float specularPower;
    float specularIntensity;
    float buffer;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

//specular power range
static const float2 g_SpecPowerRange = { 0.1, 250.0 };

//this is where we output to each render target
struct PS_GBUFFER_OUT
{
    float4 ColorSpecInt: SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 SpecPow: SV_TARGET2;
};

//func to pack
PS_GBUFFER_OUT PackGBuffer( float3 BaseColor, float3 Normal, float
    SpecIntensity, float SpecPower, float emissive )
{
    PS_GBUFFER_OUT Out;
    // Normalize the specular power
    float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;

    // Pack all the data into the GBuffer structure
    Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
    Out.Normal = float4(Normal.xyz * 0.5 + 0.5, 0.0);
    Out.SpecPow = float4(SpecPowerNorm, emissive, 0.0, 0.0);
    return Out;
}

PS_GBUFFER_OUT main( PixelInputType input ) : SV_TARGET
{
    float3 baseColor = shaderTexture.Sample( SampleType, input.uv );
    float3 normal = input.normal.xyz;

    PS_GBUFFER_OUT buff = PackGBuffer( baseColor, normal, specularIntensity, specularPower, emissive );


    return buff;
}