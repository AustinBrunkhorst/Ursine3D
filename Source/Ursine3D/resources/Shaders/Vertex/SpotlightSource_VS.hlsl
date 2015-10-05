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
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float4 cpPos : TEXCOORD0;
};


PixelInputType main( VertexInputType input )
{
    PixelInputType output;

    // 
    input.position.w = 1.f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.Position = mul( input.position, transform );
    output.Position = mul( output.Position, viewMatrix );
    output.Position = mul( output.Position, projectionMatrix );

    output.cpPos = output.Position;

    return output;
}