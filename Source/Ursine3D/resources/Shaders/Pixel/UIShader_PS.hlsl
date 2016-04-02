//texture
Texture2D gColorTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);

struct PS_INPUT
{
  float4 position : SV_POSITION;
  float2 uv : UV;
};

float4 main( PS_INPUT input ) : SV_TARGET
{
  float4 color = gColorTexture.Sample(SampleType, input.uv);
    
  return color.zyxw; 
}