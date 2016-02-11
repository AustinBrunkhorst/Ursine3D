cbuffer CameraBuffer : register(b0)
{
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer TransformBuffer : register(b1)
{
  matrix transform;
}

struct VertexInputType
{
  float4 position : POSITION;
  float4 normal : NORMAL;
  float2 uv : UV;
  //@matt don't forget this
};

struct GSInputType
{
  float4 position : SV_POSITION;
};

GSInputType main( VertexInputType input )
{
  GSInputType output;
  output.position = mul( float4(0,0,0,1), transform );

  return output;
}