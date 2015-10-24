//texture
Texture2D colorTexture : register(t0);
Texture2D lightMap : register(t1);

//sample type
SamplerState SampleType : register(s0);

struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 uv : UV;
};

float4 main(PixelInputType input) : SV_TARGET
{
  float4 lightColor;
  float4 textureColor;

  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColor = colorTexture.Sample(SampleType, input.uv);

  //get the color of the light
  lightColor = lightMap.Sample(SampleType, input.uv);


  //get the diffuse light from the xyz
  float3 diffuseLight = lightColor.xyz;
  float specular = lightColor.w;

  //return the light color
  return float4(textureColor.xyz * lightColor.xyz + specular, 1);
}