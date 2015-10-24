cbuffer CameraBuffer : register(b0)
{
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer TransformBuffer : register(b1)
{
  matrix transform;
}

cbuffer PrimColorBuffer : register(b5)
{
  float4 color;
}

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : UV;
  float4 depth : DEPTH;
};

float rand_1_05( float2 uv )
{
  float2 noise = (frac( sin( dot( uv * 20, float2(12.9898, 78.233)*2.0 ) ) * 43758.5453 ));
  return abs( noise.x + noise.y ) * 0.5;
}

[maxvertexcount(3)]
void main(
	triangle PixelInputType input[3] : SV_POSITION,
	inout TriangleStream< PixelInputType > output
)
{
  float4 target = float4(4.5 + (sin( color.z * 10) * 3), 4.5 , -cos( color.z * 10 ) * 3 , 1);

  //1. Get the average position
  //average the three positions
  float4 averagePos = input[ 0 ].position + input[ 1 ].position + input[ 2 ].position;
  averagePos /= 3;

  //check if we should skip
  float4 worldPos = mul( averagePos, transform );
  float dist = length( target - worldPos );

  float scalar = 1;

  //dist is distance to point, color.y is the radius
  scalar = max(color.y - dist, 0);

  //Find the rotation
  //face rotation
  float angle = (rand_1_05(input[0].uv) + color.z * 0.2f) * scalar;
  matrix rotation = 
  { cos( angle ), -sin( angle ), 0, 0,
    sin( angle ), cos( angle ), 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1 };

  float angle2 = (rand_1_05( input[ 1 ].uv ) + color.z * 0.2f) * scalar;
  matrix rotation2 =
  { cos( angle2 ), 0, -sin( angle2 ), 0,
    0, 1, 0, 0,
    sin( angle2 ), 0, cos( angle2 ), 0,
    0, 0, 0, 1 };

  float angle3 = (rand_1_05( input[ 2 ].uv ) + color.z * 0.2f) * scalar;
  matrix rotation3 =
  { 
    1, 0, 0, 0,
    0, cos( angle3 ), -sin( angle3 ), 0,
    0, sin( angle3 ), cos( angle3 ), 0,
    0, 0, 0, 1 };

  //Transform face back, rotation, translate back
  for (int y = 0; y < 3; ++y)
  {
    input[ y ].position -= averagePos;
    input[ y ].position = mul( input[ y ].position, rotation );
    input[ y ].position = mul( input[ y ].position, rotation2 );
    input[ y ].position = mul( input[ y ].position, rotation3 );
    input[ y ].position += averagePos;
  }

  //2. Find the offset for inflation, vector to target
  averagePos = input[ 0 ].position + input[ 1 ].position + input[ 2 ].position;
  averagePos /= 3;

  //transform
  for (int z = 0; z < 3; ++z)
  {
    input[ z ].position = mul( input[ z ].position, transform );
  }

  //2. Find the offset for vector to target
  averagePos = input[ 0 ].position + input[ 1 ].position + input[ 2 ].position;
  averagePos /= 3;
  
  float4 vec = normalize( target - averagePos ) * color.x;

  //5. add the vector to target
  [unroll]
  for (int x = 0; x < 3; ++x)
  {
    //modify stuff
    input[ x ].position += vec * (scalar);

    //calculate regular stuff
    PixelInputType outputV;

    float4 worldPos = input[ x ].position;
    float4 viewPos = mul(worldPos, viewMatrix);         //position wr2 the center of the world
    outputV.depth = viewPos;                             //position wr2 the camera
    outputV.position = mul(viewPos, projectionMatrix);   //get the screen pos
    outputV.normal = mul( input[ x ].normal, transform);
    outputV.normal = mul(outputV.normal, viewMatrix);

    outputV.uv = input[ x ].uv;

    output.Append( outputV );
  }
}