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
    float3  Pos         : POSITION;
    float3  Nor         : NORMAL;
    float3  Tan         : TANGENT;
    float2  Tex         : TEXCOORD;
    float4  BWeight     : BLENDWEIGHT;
    uint4   BIdx        : BLENDINDICES;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float4 cpPos : TEXCOORD0;
};


PixelInputType main( VertexInputType input )
{
    PixelInputType output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.Position = mul( float4(input.Pos, 1.0f), transform );
    output.Position = mul( output.Position, viewMatrix );
    output.Position = mul( output.Position, projectionMatrix );

    output.cpPos = output.Position;

    return output;
}