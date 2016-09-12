cbuffer CameraBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};

cbuffer TransformBuffer : register(b1)
{
    matrix World;
}

struct VS_INPUT
{
    float3  Pos         : POSITION;
    float3  Nor         : NORMAL;
    float3  Tan         : TANGENT;
    float2  Tex         : TEXCOORD;
    float4  BWeight     : BLENDWEIGHT;
    uint4   BIdx        : BLENDINDICES;
};


struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : UV;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 worldPos = mul(float4(input.Pos, 1), World);
    float4 viewPos = mul(worldPos, View);         //position wr2 the center of the world

    output.Pos = mul(viewPos, Projection);   //get the screen pos

    output.Tex = input.Tex;

    return output;
}