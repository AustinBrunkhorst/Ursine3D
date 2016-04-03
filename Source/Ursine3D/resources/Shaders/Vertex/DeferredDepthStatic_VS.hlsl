cbuffer CameraBuffer : register(b0)
{
    matrix View;
    matrix Projection;
};

cbuffer TransformBuffer : register(b1)
{
    matrix World;
}

cbuffer MatrixStack : register(b12)
{
    matrix matPal[ 96 ];
}

cbuffer TextureOffset : register(b13)
{
    float2 textureOffset;
    float2 buffer;
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
    float4 Pos          : SV_POSITION;
    float4 normal       : NORMAL;
    float2 Tex          : UV;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.Pos = mul(float4(input.Pos.xyz, 1.f), World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex + textureOffset;

    output.normal = mul(float4(input.Nor.xyz, 0), World);
    output.normal = normalize(mul(output.normal, View));

    return output;
}