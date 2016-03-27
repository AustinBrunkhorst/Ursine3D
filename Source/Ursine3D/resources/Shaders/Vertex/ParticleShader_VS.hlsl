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
    float4 color : COLOR;
    float2 uv : UV;
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

    switch ( vertexInQuad )
    {
    case 0:
        position.x = -1.f;
        position.y = 1.f;
        position.z = 0.f;

        output.uv.x = 0;
        output.uv.y = 0;
        break;
    case 1:
        position.x = 1.f;
        position.y = 1.f;
        position.z = 0.f;

        output.uv.x = 1;
        output.uv.y = 0;
        break;
    case 2:
        position.x = -1.f;
        position.y = -1.f;
        position.z = 0.f;

        output.uv.x = 0;
        output.uv.y = 1;
        break;
    case 3:
        position.x = 1.f;
        position.y = 1.f;
        position.z = 0.f;

        output.uv.x = 1;
        output.uv.y = 0;
        break;
    case 4:
        position.x = 1.f;
        position.y = -1.f;
        position.z = 0.f;

        output.uv.x = 1;
        output.uv.y = 1;
        break;
    default:
        position.x = -1.f;
        position.y = -1.f;
        position.z = 0.f;

        output.uv.x = 0;
        output.uv.y = 1;
        break;
    }

    // scale
    position.xy *= g_bufPosColor[ particleIndex ].scaleX * 0.5f;

    // rotate
    position.xy = mul(position.xy, GenerateRotation(g_bufPosColor[ particleIndex ].rotation[ 0 ]));

    // into world -> translate
    position = mul(position, (float3x3)invView) + mul(float4(g_bufPosColor[ particleIndex ].position, 1.0f), World).xyz;

    output.position = mul(float4(position, 1.0f), view);
    output.position = mul(output.position, projection);
    output.color = (g_bufPosColor[ particleIndex ].color * g_bufPosColor[ particleIndex ].color.w) * color;

    return output;
}

//cbuffer CameraBuffer : register(b0)
//{
//    matrix view;
//    matrix projection;
//};
//
//cbuffer invView : register(b4)
//{
//    matrix invView;
//    float nearPlane;
//    float farPlane;
//};
//
//struct Particle_GPU
//{
//    // alignment is really, really important
//    // boundry ////////////
//    float3 position;
//    float scaleX;
//
//    // boundry ////////////
//    float3 rotation;
//    float scaleY;
//
//    // boundry ////////////
//    float4 color;
//};
//
//cbuffer ParticleData : register(b13)
//{
//    Particle_GPU g_bufPosColor[ 1024 ];
//}
//
//// dummy for reflection data
//struct VertexInputType
//{
//    float4 position : POSITION;
//};
//
//struct PixelInputType
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//};
//
//PixelInputType main(uint id : SV_VERTEXID)
//{
//    PixelInputType output;
//
//    uint particleIndex = id / 4;
//    uint vertexInQuad = id % 4;
//
//    float3 position;
//
//    position.x = (vertexInQuad % 2) ? 1.0f : -1.0f;
//    position.y = (vertexInQuad & 2) ? -1.0f : 1.0f;
//    position.z = 0;
//    position.xy *= g_bufPosColor[ particleIndex ].scaleX;
//
//    position = g_bufPosColor[ particleIndex ].position; // mul(position, (float3x3)view) + g_bufPosColor[ particleIndex ].position
//
//    output.position = mul(float4(position, 1.0f), view);
//    output.position = mul(output.position, projection);
//    output.color = g_bufPosColor[ particleIndex ].color;
//
//    //output.tex.x = (vertexInQuad % 2) ? 1.0f : 0.0f;
//    //output.tex.y = (vertexInQuad & 2) ? 1.0f : 0.0f;
//
//    return output;
//}