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
    float3 normal = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        pos += weights[ i ] * mul(float4(input.Pos.xyz, 1.0f), matPal[ indices[ i ] ]).xyz;
        normal += weights[ i ] * mul(float4(input.Nor.xyz, 0.0f), matPal[ indices[ i ] ]).xyz;
    }

    normal = normalize(normal);

    output.normal = mul(float4(input.Nor.xyz, 0), World);
    output.normal = normalize(mul(output.normal, View));

    output.Pos = mul(float4(pos.xyz + normal * 0.5, 1.0f), World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos + float4(0, 0, 1, 0), Projection);

    output.Tex = input.Tex + textureOffset;

    return output;
}