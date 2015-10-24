cbuffer CameraBuffer : register(b0)
{
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer camInfo : register(b6)
{
  float4 camPos;
  float4 camUp;
};

cbuffer SpriteInfo : register(b7)
{
  float width;
  float height;
}

//input into geometry shader
struct VertexInputType
{
  float4 position : SV_POSITION;
  float4 color : COLOR;
  float size : SIZE;
};

//output from geometry shader (goes to pixel shader)
struct GSOutput
{
  float4 pos : SV_POSITION;
  float2 uv : UV;
};

//max number of vertices will be outputed
[maxvertexcount( 6 )]
void main(
  point VertexInputType input[ 1 ] : SV_POSITION, //the input (what type of input, what the vertex looks like
  inout TriangleStream< GSOutput > output       //output
  )
{
  //calculate normal to the plane
  float3 look = ((camPos - input[ 0 ].position)).xyz;
  float distance = length( look );
  look = normalize( look );

  float halfWidth = width / 2.f;
  float halfHeight = height / 2.f;

  //crossing the up and plane gets us the right
  float3 rightVector = cross( camUp.xyz, look.xyz );
  float3 upVector = cross( look, rightVector );

  //scale by width/height
  rightVector *= halfWidth;
  upVector *= halfHeight;

  //calculate vertices
  GSOutput outputVert[ 4 ];
  outputVert[ 0 ].pos = float4(input[ 0 ].position.xyz - rightVector + upVector, 1); //tl
  outputVert[ 0 ].uv = float2(0, 0);
  outputVert[ 1 ].pos = float4(input[ 0 ].position.xyz + rightVector + upVector, 1); //tr
  outputVert[ 1 ].uv = float2(1, 0);
  outputVert[ 2 ].pos = float4(input[ 0 ].position.xyz - rightVector - upVector, 1); //bl
  outputVert[ 2 ].uv = float2(0, 1);
  outputVert[ 3 ].pos = float4(input[ 0 ].position.xyz + rightVector - upVector, 1); //br
  outputVert[ 3 ].uv = float2(1, 1);

                                                                                     //move from world space to screen space
  for (int x = 0; x < 4; ++x)
  {
    outputVert[ x ].pos = mul( outputVert[ x ].pos, viewMatrix );
    outputVert[ x ].pos = mul( outputVert[ x ].pos, projectionMatrix );
  }

  //finally, add the vertices to the list, COUNTER CLOCKWISE
  //rendering top right poly
  output.Append( outputVert[ 2 ] );
  output.Append( outputVert[ 1 ] );
  output.Append( outputVert[ 0 ] );

  //restart for new triangle
  output.RestartStrip( );

  //render bottom right poly
  output.Append( outputVert[ 2 ] );
  output.Append( outputVert[ 3 ] );
  output.Append( outputVert[ 1 ] );
}