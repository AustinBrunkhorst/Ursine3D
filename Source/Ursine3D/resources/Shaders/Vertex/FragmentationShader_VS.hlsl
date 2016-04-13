cbuffer TextureOffset : register(b13)
{
    float2 textureOffset;
    float2 buffer;
}

cbuffer MatrixStack : register(b12)
{
    matrix matPal[ 96 ];
}

cbuffer FragData : register(b11)
{
    // normal offset scalar
    float normalOffset;
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
  float4 position : SV_POSITION;
  float4 basePos : BASE_POS;
  float4 normal : NORMAL;
  float2 uv : UV;
};

PixelInputType main( VertexInputType input )
{
  PixelInputType output;

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

  output.position = float4(pos + normal * normalOffset, 1.0f);
  output.basePos = float4(input.Pos + normal * normalOffset, 1.0f);
  output.normal = float4(normal, 0.0f);
  output.uv = input.Tex + textureOffset;

  return output;
}