cbuffer PrimColorBuffer : register(b5)
{
  float4 color;
}

struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 uv : UV;
};

float4 main( PixelInputType input ) : SV_TARGET
{
  return color;
}