// texture
Texture2D gShaderTexture    : register(t0);
Texture2D gNormalMap        : register(t1);

// sample type
SamplerState SampleType : register(s0);

cbuffer MaterialBuffer : register(b10)
{
    float emissive;
    float specularPower;
    float specularIntensity;
    int objID;
};

struct PS_INPUT
{
    float4 Pos          : SV_POSITION;
    float4 normal       : NORMAL;
    float4 tangent      : TANGENT;
    float4 bitangent    : BITANGENT;
    float2 uv           : UV;
};

// specular power range
static const float2 cSpecPowerRange = { 0.1, 250.0 };

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
    float SpecPowerNorm = (SpecPower - cSpecPowerRange.x) / cSpecPowerRange.y;

    // convert id into proper sizes
    int word1 = objID & 0xff;           //first 8 bits
    int word2 = (objID >> 8) & 0xff;  //second 8 bits

                                      // Pack all the data into the GBuffer structure
    Out.ColorSpecInt = float4(BaseColor);
    Out.Normal = float4(Normal.xyz * 0.5 + 0.5, emissive);
    //                                            first 2 are handle index      last 8 is type
    Out.SpecPow = float4(SpecPowerNorm, word1 / 255.f, word2 / 255.f, SpecIntensity);

    // return
    return Out;
}

PS_GBUFFER_OUT main(PS_INPUT input)
{
    float4 baseColor = float4(gShaderTexture.Sample(SampleType, input.uv).xyz * color.xyz, color.a);
    float3 normal = input.normal.xyz;

    // sample
    float3 normalValue = (gShaderTexture.Sample(SampleType, input.uv).xyz);

    float3x3 texSpace = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz);

    // calculate final normal based upon bitan tan and normal
    float3 finalNormal = normalize( mul(normalValue, texSpace) );

    PS_GBUFFER_OUT buff = PackGBuffer(baseColor, finalNormal, specularIntensity, specularPower, emissive);


    return buff;
}