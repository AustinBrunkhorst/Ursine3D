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

cbuffer InvProj : register(b4)
{
    float4x4 InvView;
    float nearPlane;
    float farPlane;
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

float4 calculateColor(float3 worldNormal)
{
    float up = worldNormal.y * 0.5 + 0.5;

    return float4(up, up, up, 1);
}

// func to pack
PS_GBUFFER_OUT PackGBuffer(float4 BaseColor, float3 Normal, float
    SpecIntensity, float SpecPower, float emissive)
{
    PS_GBUFFER_OUT Out;

    // convert id into proper sizes
    int word1 = objID & 0xff;           //first 8 bits
    int word2 = (objID >> 8) & 0xff;    //second 8 bits
     
                                      // Pack all the data into the GBuffer structure
    Out.ColorSpecInt = calculateColor(normalize(mul(Normal, (float3x3)(InvView)).xyz)) * BaseColor;
    Out.ColorSpecInt = saturate(1.0f - emissive) * Out.ColorSpecInt + emissive * BaseColor;

    // doesn't matter
    Out.Normal = float4(0, 0, 1, 0);
    
    // set this
    Out.SpecPow = float4(0, word1 / 255.f, word2 / 255.f, SpecIntensity);

    // return
    return Out;
}

PS_GBUFFER_OUT main(PixelInputType input)
{
    float4 baseColor = float4(shaderTexture.Sample(SampleType, input.uv).xyz * color.xyz, color.a);
    float3 normal = input.normal.xyz;

    PS_GBUFFER_OUT buff = PackGBuffer(baseColor, normal, specularIntensity, specularPower, emissive);


    return buff;
}