//texture
Texture2D colorTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 texColor = colorTexture.Sample(SampleType, input.uv);

    if ( texColor.x <= 0 )
        discard;

    return float4(input.color.xyz * texColor.x, texColor.x);
}