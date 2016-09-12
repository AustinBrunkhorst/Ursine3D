cbuffer CameraBuffer : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer TransformBuffer : register(b1)
{
    matrix World;
}


cbuffer invView : register(b4)
{
    matrix invView;
    float nearPlane;
    float farPlane;
};

cbuffer posAndColor : register(b6)
{
    float4 cameraPosition;
    float4 color;
}

struct Particle_GPU
{
    // alignment is really, really important
    // boundry ////////////
    float3 position;
    float scaleX;

    // boundry ////////////
    float3 rotation;
    float scaleY;

    // boundry ////////////
    float4 color;
};

cbuffer ParticleData : register(b13)
{
    Particle_GPU g_bufPosColor[ 1024 ];
}

// dummy for reflection data
struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : UV;
    float zDepth : DEPTH;
};

float2x2 GenerateRotation(float radianAngle)
{
    return float2x2(float2(cos(radianAngle), -sin(radianAngle)), (float2(sin(radianAngle), cos(radianAngle))));
}

PixelInputType main(uint id : SV_VERTEXID)
{
    PixelInputType output;

    uint particleIndex = id / 6;
    uint vertexInQuad = id % 6;

    float3 position;

    // scale
    float3 rightVec = mul(mul(normalize(g_bufPosColor[ particleIndex ].rotation.xyz), (float3x3)World).xyz, (float3x3)view).xyz;
    float3 upVec = cross(rightVec, float3(0, 0, -1));


    upVec.z = 0;
    rightVec.z = 0;

    switch ( vertexInQuad )
    {
    case 0:
        position = -rightVec + upVec;

        output.uv.x = 0;
        output.uv.y = 0;
        break;
    case 1:
        position = rightVec + upVec;

        output.uv.x = 1;
        output.uv.y = 0;
        break;
    case 2:
        position = -rightVec - upVec;

        output.uv.x = 0;
        output.uv.y = 1;
        break;
    case 3:
        position = rightVec + upVec;

        output.uv.x = 1;
        output.uv.y = 0;
        break;
    case 4:
        position = rightVec - upVec;

        output.uv.x = 1;
        output.uv.y = 1;
        break;
    default:
        position = -rightVec - upVec;

        output.uv.x = 0;
        output.uv.y = 1;
        break;
    }

    position.xy *= (g_bufPosColor[ particleIndex ].scaleX) * 0.5f;

    // into world -> translate
    
    position = mul(position, (float3x3)invView) + mul(float4(g_bufPosColor[ particleIndex ].position, 1.0f), World).xyz;

    output.position = mul(float4(position, 1.0f), view);
    output.position = mul(output.position, projection);
    output.color = (g_bufPosColor[ particleIndex ].color * g_bufPosColor[ particleIndex ].color.w) * color;
     
    output.zDepth = output.position.z;

    return output;
}