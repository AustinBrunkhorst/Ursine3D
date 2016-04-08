//texture
Texture2D gDepthTexture         : register(t0);
Texture2D gColorTexture     : register(t1);

//sample type
SamplerState SampleType     : register(s0);

//inv projection 
cbuffer InverseProjection   : register(b4)
{
    float       gAValue;
    float       gBValue;
    float2      buffer;
};

cbuffer ShadowFalloff               : register(b12)
{
    float gLightStep;
    float gBorderCutoff;
    float2 buffert;
}

// convert given depth value from exponential to linear
float LinearizeDepth(float depth)
{
    return gBValue / (depth - gAValue);
}


struct PS_INPUT
{
    float4  position    : SV_POSITION;
    float4  color       : COLOR;
    float2  uv          : UV;
    float   zDepth      : DEPTH;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float sceneDepth = LinearizeDepth(gDepthTexture.Load(int3(input.position.xy, 0)).x );
    float particleDepth = LinearizeDepth(input.position.z);

    // discard particle if occluded
    if(sceneDepth < particleDepth) discard;

    float4 texColor = gColorTexture.Sample(SampleType, input.uv);

    float depthScalar = (sceneDepth - particleDepth) * 150.0f;

    float Output = 0.5 *saturate(2 * ((depthScalar > 0.5) ? 1 - depthScalar : depthScalar));
    Output = (depthScalar > 0.5) ? 1 - Output : Output;

    return (input.color * texColor) * (Output);
}