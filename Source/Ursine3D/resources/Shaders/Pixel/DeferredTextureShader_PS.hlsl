//texture
Texture2D gColorTexture : register(t0);
Texture2D lightMap : register(t1);

//sample type
SamplerState SampleType : register(s0);

struct PS_INPUT
{
  float4 position : SV_POSITION;
  float2 uv : UV;
};

float4 main(PS_INPUT input) : SV_TARGET
{
  float4 lightColor;
  float4 textureColor;

  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColor = gColorTexture.Load(int3(input.position.xy, 0));

  //get the color of the light
  lightColor = lightMap.Load( int3(input.position.xy, 0) );

  //get the diffuse light from the xyz
  float3 diffuseLight = lightColor.xyz;
  float specular = lightColor.w;

  return float4(lightColor.xyz, 1);
  //return the light color
  //return float4(textureColor.xyz * lightColor.xyz + specular, 1);
}