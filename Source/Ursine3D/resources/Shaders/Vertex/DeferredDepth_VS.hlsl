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

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : UV;
};

PixelInputType main(VertexInputType input)
{
  PixelInputType output;

  // 
  input.position.w = 1.f;

  // Calculate the position of the vertex against the world, view, and projection matrices.
  float4 worldPos = mul(input.position, transform);
  float4 viewPos = mul(worldPos, viewMatrix);         //position wr2 the center of the world

  output.position = mul(viewPos, projectionMatrix);   //get the screen pos
  output.normal = mul(input.normal, transform);
  output.normal = mul(output.normal, viewMatrix);

  output.uv = input.uv;

  return output;
}