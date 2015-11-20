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
	matrix matPal[96];
}

//struct VertexInputType
//{
//  float4 position : POSITION;
//  float4 normal : NORMAL;
//  float2 uv : UV;
//  //@matt don't forget this
//};

struct VS_INPUT
{
	float3	Pos		: POSITION;
	float3	Nor		: NORMAL;
	float2	Tex		: TEXCOORD;
	float4	BWeight : BLENDWEIGHT;
	uint4	BIdx	: BLENDINDICES;
};


struct VS_OUTPUT
{
  float4 Pos : SV_POSITION;
  float4 normal : NORMAL; 
  float2 Tex : UV;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.BWeight.x;
	weights[1] = input.BWeight.y;
	weights[2] = input.BWeight.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
	
	int indices[4] = { 0, 0, 0, 0 };
	indices[0] = input.BIdx.x;
	indices[1] = input.BIdx.y;
	indices[2] = input.BIdx.z;
	indices[3] = input.BIdx.w;
	
	float3 pos = float3(0.f, 0.f, 0.f);
	for (int i = 0; i < 4; ++i)
	{
		pos += weights[i] * mul(float4(input.Pos.xyz, 1.0f), matPal[indices[i]]).xyz;
	}

	output.Pos = mul(float4(pos.xyz, 1.f), World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	output.normal = mul(float4(input.Nor.xyz, 0), World);
	output.normal = mul(output.normal, View);
	return output;


	// 

	//// Calculate the position of the vertex against the world, view, and projection matrices.
	//float4 worldPos = mul(float4(input.Pos, 1), World);
	//float4 viewPos = mul(worldPos, View);         //position wr2 the center of the world

	//output.Pos = mul(viewPos, Projection);   //get the screen pos
	//output.normal = mul(float4(input.Nor, 0), World);
	//output.normal = mul(output.normal, View);

	//output.Tex = input.Tex;

	//return output;
}