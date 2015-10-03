cbuffer CameraBuffer : register(b0)
{
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer TransformBuffer : register(b1)
{
  matrix transform;
}

cbuffer InvProj : register(b4)
{
  float4x4 invProj;
  float4 halfPix;
};

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
  float2 uv : UV;
};


PixelInputType main(VertexInputType input)
{
  PixelInputType output;

  // 
  input.position.w = 1.f;

  // Calculate the position of the vertex against the world, view, and projection matrices.
  output.position = mul(input.position, transform);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  output.uv = input.uv;

  return output;
}