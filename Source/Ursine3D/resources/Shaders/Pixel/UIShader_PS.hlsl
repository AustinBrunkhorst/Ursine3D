//texture
Texture2D colorTexture : register(t0);

//sample type
SamplerState SampleType : register(s0);

struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 uv : UV;
};

float4 main( PixelInputType input ) : SV_TARGET
{
  float4 color = colorTexture.Load( int3(input.position.xy, 0) );
  //float4 color = colorTexture.Sample(SampleType, input.uv);
  return float4(color.z, color.y, color.x, color.w);
}