Texture2D gColorTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);


cbuffer PrimColorBuffer : register(b5)
{
  float4 color;
}

struct PS_INPUT
{
  float4 position : SV_POSITION;
  float2 uv : UV;
};

float4 main( PS_INPUT input ) : SV_TARGET
{
  return color;
}