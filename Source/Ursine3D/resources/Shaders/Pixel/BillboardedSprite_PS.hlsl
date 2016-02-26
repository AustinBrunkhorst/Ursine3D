// texture
Texture2D shaderTexture : register(t0);

// sample type
SamplerState SampleType : register(s0);

cbuffer MaterialBuffer : register(b10)
{
    float emissive;
    float specularPower;
    float specularIntensity;
    int objID;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

// specular power range
static const float2 g_SpecPowerRange = { 0.1, 250.0 };

// this is where we output to each render target
struct PS_GBUFFER_OUT
{
    float4 ColorSpecInt: SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 SpecPow: SV_TARGET2;
};

cbuffer PrimColorBuffer : register(b5)
{
    float4 color;
}

// func to pack
PS_GBUFFER_OUT PackGBuffer(float4 BaseColor, float3 Normal, float
    SpecIntensity, float SpecPower, float emissive)
{
    PS_GBUFFER_OUT Out;
    // Normalize the specular power
    float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;

    // convert id into proper sizes
    int word1 = objID & 0xff;           //first 8 bits
    int word2 = (objID >> 8) & 0xff;  //second 8 bits

                                      // Pack all the data into the GBuffer structure
    Out.ColorSpecInt = float4(BaseColor);
    Out.Normal = float4(Normal.xyz * 0.5 + 0.5, emissive);
    //                                            first 2 are handle index      last 8 is type
    Out.SpecPow = float4(1.0f, word1 / 255.f, word2 / 255.f, SpecIntensity);

    // return
    return Out;
} 

PS_GBUFFER_OUT main(PixelInputType input)
{
    float4 baseColor = float4(shaderTexture.Sample(SampleType, input.uv));
    float3 normal = input.normal.xyz;

    if ( baseColor.w <= 0 )
        discard;

    PS_GBUFFER_OUT buff = PackGBuffer(baseColor, normal, specularIntensity, specularPower, emissive);


    return buff;
}