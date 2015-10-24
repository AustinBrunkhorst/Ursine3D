//texture
Texture2D shaderTexture : register(t0);

Texture2D normalMap : register(t1);

//sample type
SamplerState SampleType : register(s0);

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : UV;
  float4 depth : DEPTH;
};

//this is where we output to each render target
struct PixelOutputType
{
  float4 depth  : SV_Target0;
  float4 normal : SV_Target1;
  float4 color  : SV_Target2;
};

PixelOutputType main( PixelInputType input )
{
  //these need to be stored
  float specularPower = 1;
float specularIntensity = 1;

PixelOutputType output;

// Sample the color from the texture and store it for output to the render target.
output.color = float4(shaderTexture.Sample( SampleType, input.uv ).xyz, specularPower);

// Store the normal for output to the render target.
output.normal = float4(normalize( input.normal ).xyz, specularPower);

output.depth = float4(input.depth.xyz, specularIntensity);

return output;
}