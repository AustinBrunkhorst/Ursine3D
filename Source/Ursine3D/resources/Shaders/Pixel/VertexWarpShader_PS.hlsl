//texture
Texture2D gColorTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 texColor = gColorTexture.Sample(SampleType, input.uv);
    return input.color * texColor;
}