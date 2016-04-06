#include "../Headers/Randomness_H.hlsl"

cbuffer CameraBuffer : register(b0)
{
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer TransformBuffer : register(b1)
{
  matrix transform;
}

cbuffer FragData : register(b11)
{
    // vertical force value
    float verticalForce;

    // horizontal force value
    float horizontalForce;

    // outward force value
    float outwardForce;

    // gravity force value
    float gravityForce;

    // time
    float time;

    float randomForce;

    float spinSpeed;
}

struct GS_INPUT
{
  float4 position : SV_POSITION;
  float4 basePos : BASE_POS;
  float4 normal : NORMAL;
  float2 uv : UV;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
    float4 color : COLOR;
};

float RandomNormFloat(float2 xy)
{
    return (random(xy, 2.0) - 0.5f) * 2.0f;
}

float rand_1_05(float2 uv)
{
    float2 noise = (frac(sin(dot(uv * 20, float2(12.9898, 78.233)*2.0)) * 43758.5453));
    return (noise.x + noise.y) * 0.5;
}


[maxvertexcount(3)]
void main(
    triangle GS_INPUT input[3] : SV_POSITION,
    inout TriangleStream< GS_OUTPUT > output
)
{
    /////////////////////////////////////////////////////////////////////////////////////
    //Get the average positions
    //average the three positions
    float4 averagePos = input[ 0 ].basePos + input[ 1 ].basePos + input[ 2 ].basePos;
    averagePos /= 3;

    float4 averageTruePos = input[ 0 ].position + input[ 1 ].position + input[ 2 ].position;
    averageTruePos /= 3;

    // pregenerate random values to optimize calls
    float randXY = RandomNormFloat(averagePos.xy);
    float randYZ = RandomNormFloat(averagePos.yz);
    float randXZ = RandomNormFloat(averagePos.xz);

    // calculate the required vectors for up and side trajectories
    float4 upVec        = normalize( mul(
                                    float4(0, averagePos.y, 0, 0), 
                                    transform
                                ) 
                            );

    float4 sideVec      = normalize( mul(
                                    float4(averagePos.x, 0, averagePos.z, 0), 
                                    transform
                                ) 
                            );

    float4 outVec       = normalize( mul(
                                    float4(averagePos.xyz, 0), 
                                    transform
                                ) 
                            );

    float4 randomVec    = normalize( float4( randYZ, randXZ, randXY, 0 ) );

    /////////////////////////////////////////////////////////////////////////////////////
    // generate rotations
    float angleScalar = time * 0.5f * spinSpeed;
    float angle = (averagePos.x * randXY) * angleScalar;
    float2x2 rotation =
    { 
        cos( angle ), -sin( angle ),
        sin( angle ),  cos( angle ) 
    };

    float angle2 = -(averagePos.y * randYZ) * angleScalar;
    float2x2 rotation2 =
    { 
        cos( angle2 ), -sin( angle2 ),
        sin( angle2 ),  cos( angle2 )
    };

    float angle3 = (averagePos.z * randXZ) * angleScalar;
    float2x2 rotation3 =
    { 
        cos( angle3 ), -sin( angle3 ),
        sin( angle3 ),  cos( angle3 ) 
    };

    //Transform face to origin, rotate, translate back
    for (int y = 0; y < 3; ++y)
    {
      input[ y ].position -= averageTruePos;
      input[ y ].position.xy = mul( input[ y ].position.xy, rotation );
      input[ y ].position.xz = mul( input[ y ].position.xz, rotation2 );
      input[ y ].position.yz = mul( input[ y ].position.yz, rotation3 );
      input[ y ].position += averageTruePos;

      input[ y ].normal.xy = normalize(mul(input[ y ].normal.xy, rotation));
      input[ y ].normal.xz = normalize(mul(input[ y ].normal.xz, rotation2));
      input[ y ].normal.yz = normalize(mul(input[ y ].normal.yz, rotation3));
    }

    /////////////////////////////////////////////////////////////////////////////////////
    // apply trajectories to this triangle
    float upTime = (-time * time * gravityForce + time) * verticalForce;

    //transform
    [ unroll ]
    for (int z = 0; z < 3; ++z)
    {
        input[ z ].position = mul(input[ z ].position, transform);
        input[ z ].position +=  ( abs(upVec) * upTime                   ) + // up vec
                                ( sideVec   * time * horizontalForce    ) + // horizontal force
                                ( outVec    * time * outwardForce       ) + // outward force
                                ( randomVec * time * randomForce        );  // random force
    }

    /////////////////////////////////////////////////////////////////////////////////////
    // add the vector to target
    [ unroll ]
    for (int x = 0; x < 3; ++x)
    {
        //calculate regular stuff
        GS_OUTPUT outputV;

        float4 worldPos = input[ x ].position;
        float4 viewPos = mul(worldPos, viewMatrix);             //position wr2 the center of the world
        outputV.position = mul(viewPos, projectionMatrix);      //get the screen pos
        outputV.normal = mul(input[ x ].normal, transform);
        outputV.normal = mul(outputV.normal, viewMatrix);
        outputV.color = averagePos;
        outputV.uv = input[ x ].uv;

        output.Append(outputV);
    }
}