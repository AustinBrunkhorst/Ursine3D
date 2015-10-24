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
  float4 color : COLOR;
  float size : SIZE;
};

struct VertexOutputType
{
  float4 position : SV_POSITION;
  float4 color : COLOR;
  float size : SIZE;
};

VertexOutputType main( VertexInputType input )
{
  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.f;

  return input;
}