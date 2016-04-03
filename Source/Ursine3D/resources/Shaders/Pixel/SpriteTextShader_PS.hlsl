//texture
Texture2D gColorTexture : register(t0);

//sample type
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
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : UV;
};

struct PS_GBUFFER_OUT
{
    float4 ColorSpecInt : SV_TARGET0;
    float4 Normal       : SV_TARGET1;
    float4 SpecPow      : SV_TARGET2;
};

// func to pack
PS_GBUFFER_OUT PackGBuffer(
    float4 BaseColor, 
    float3 Normal, 
    float SpecIntensity, 
    float SpecPower, 
    float emissive
)
{
    PS_GBUFFER_OUT Out;
    // Normalize the specular power
    float SpecPowerNorm = 0;

    // convert id into proper sizes
    int word1 = objID & 0xff;           //first 8 bits
    int word2 = (objID >> 8) & 0xff;    //second 8 bits

    // Pack all the data into the GBuffer structure
    Out.ColorSpecInt = float4( BaseColor );
    Out.Normal = float4(
        Normal.xyz * 0.5 + 0.5, 
        emissive
    );
    // first 2 are handle index      last 8 is type
    Out.SpecPow = float4(
        1.0f, 
        word1 / 255.f, 
        word2 / 255.f, 
        SpecIntensity
    );

    // return
    return Out;
}


PS_GBUFFER_OUT main(PS_INPUT input)
{
    PS_GBUFFER_OUT output;
    float4 texColor = gColorTexture.Sample( 
        SampleType, 
        input.uv 
    );

    if (texColor.x <= 0)
        discard;

    float4 finalColor = float4( 
        input.color.xyz * texColor.x, 
        texColor.x * input.color.w 
    );

    return PackGBuffer(
        finalColor, 
        float3(0, 0, 0), 
        0, 
        0, 
        1
    );
}