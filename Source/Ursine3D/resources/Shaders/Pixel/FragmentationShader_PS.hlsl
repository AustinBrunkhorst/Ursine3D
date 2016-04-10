//texture
Texture2D gColorTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);

cbuffer TextureOffset : register(b13)
{
    float2 textureOffset;
    float2 buffer;
}


cbuffer FragData : register(b11)
{
    // pulse rate
    float pulseSpeed;

    // fade
    float fadeAmount;

    // time
    float time;

    // start/end time
    float startTime;
    float endTime;

    // normal transparency
    float transparencyThreshold;

    float globalTime;

    float buffer11;

    float4 color;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
    float4 color : COLOR;
};

float4 main( PS_INPUT input ) : SV_TARGET
{
    float4 baseColor = gColorTexture.Sample( SampleType, input.uv );
    float3 normal = input.normal.xyz;

    float texLength = length(textureOffset);
    float offset = cos((globalTime) * pulseSpeed);


    // old, don't remove
    float dotp;// = saturate((1 - abs(1 - dot(normal, float3(0, 0, -1))) / (8.0f)) + transparencyThreshold + offset) * ((endTime - time) / endTime);

    dotp = saturate(1.0f - (abs(dot(normal, float3(0, 0, 1))) * fadeAmount + offset) + transparencyThreshold) * ((endTime - time) / endTime);

    return float4(baseColor.xyz * dotp * color.xyz * color.w, 1.0f);
}