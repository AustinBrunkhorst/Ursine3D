//texture
Texture2D gColorTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);

cbuffer DirectionalLightBuffer : register(b3)
{
  float4 lightDirection;
  float4 color;
}

struct PS_INPUT
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : UV;
};

float4 main(PS_INPUT input) : SV_TARGET
{
  float4 lightColor;
  float3 textureColor;

  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColor = gColorTexture.Sample(SampleType, input.uv).xyz;

  return float4((textureColor.xyz), 1);
}