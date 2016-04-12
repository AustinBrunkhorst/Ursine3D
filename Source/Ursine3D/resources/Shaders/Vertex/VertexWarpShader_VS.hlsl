#include "../Headers/Randomness_H.hlsl"

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
    float2 diffuseTextureOffset;
    float2 diffuseTextureScalar;

    float2 emissiveTextureOffset;
    float2 emissiveTextureScalar;
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
    float4 tangent      : TANGENT;
    float4 bitangent    : BITANGENT;
    float2 Tex          : UV;
    float2 emisiveTex          : EM_UV;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float weights[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[ 0 ] = input.BWeight.x;
    weights[ 1 ] = input.BWeight.y;
    weights[ 2 ] = input.BWeight.z;
    weights[ 3 ] = 1.0f - weights[ 0 ] - weights[ 1 ] - weights[ 2 ];

    int indices[ 4 ] = { 0, 0, 0, 0 };
    indices[ 0 ] = input.BIdx.x;
    indices[ 1 ] = input.BIdx.y;
    indices[ 2 ] = input.BIdx.z;
    indices[ 3 ] = input.BIdx.w;

    float3 pos = float3(0.f, 0.f, 0.f);

    for (int i = 0; i < 4; ++i)
    {
        pos += weights[ i ] * mul(float4(input.Pos.xyz, 1.0f), matPal[ indices[ i ] ]).xyz;
    }

    // play with it...
    float x = sin((pos.x + diffuseTextureOffset.x / emissiveTextureScalar.y) * emissiveTextureOffset.x);
    float y = cos((pos.z + diffuseTextureOffset.x / emissiveTextureScalar.y) * emissiveTextureOffset.y) / emissiveTextureOffset.y;



    pos.y = x * y * emissiveTextureScalar.x;

    //sin(5x)*cos(5y) / 5

    // end of warping

    output.Pos = mul(float4(pos.xyz, 1.f), World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = (input.Tex + diffuseTextureOffset) * diffuseTextureScalar;
    output.emisiveTex = (input.Tex + emissiveTextureOffset) * emissiveTextureScalar;

    // move all vectors into world
    output.normal = mul(float4(input.Nor.xyz, 0), World);
    output.tangent = mul(float4(input.Tan.xyz, 0), World);

    // move all vectors into view
    output.normal = normalize(mul(output.normal, View));
    output.tangent = normalize(mul(output.tangent, View));

    // calculate bitangent
    output.bitangent = normalize(float4(cross(output.normal.xyz, output.tangent.xyz), 0));

    return output;
}