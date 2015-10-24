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

float4 main( VertexInputType input ) : SV_POSITION
{
  float4 output;
  output.w = 1;

  output = mul( input.position, transform );  //move model to world space
  output = mul( output, viewMatrix ); //move model to light space
  return output;
}