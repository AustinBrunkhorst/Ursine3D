// depth texture
Texture2D    gDepthTexture          : register(t0);
Texture2D    gIDTex          : register(t1);

// sample state
SamplerState SampleType             : register(s0);

//inv projection 
cbuffer InverseProjection           : register(b4)
{
    float4x4    gInvProj;
    float       gNearPlane;
    float       gFarPlane;
};

cbuffer ShadowFalloff               : register(b12)
{
    float gLightStep;
    float gBorderCutoff;
    float2 buffer;
}

// convert given depth value from exponential to linear
float LinearizeDepth(float depth)
{
    return (2.0f * gNearPlane) /
        (gFarPlane + gNearPlane - depth * (gFarPlane - gNearPlane));
}

int GetIDFromTex(float4 value)
{
    //read value from texture
    float4 currVal = value;// [mousePos.xy];

                           //convert to ID
    uint w1 = round(currVal.y * 255.f);
    uint w2 = round(currVal.z * 255.f);

    //construct final ID
    uint currID = w1 + (w2 << 8);

    if (currID > 73727)
    {
        return -1;
    }

    return currID;
}

/////////////////////////////////////////////////////////////////////
// STRUCTS
//input 
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 UV       : UV;
};

float4 main(PS_INPUT In) : SV_TARGET
{
    // offsets into texture
    float2 offsets[ 4 ] = {
        { -1, 0 },
        { 1, 0 },
        { 0, 1 },
        { 0, -1 }
};

float samples[ 4 ];

// DEPTH INFO
float currentPixel = LinearizeDepth(gDepthTexture.Load(int3(In.Position.xy, 0)).x);

for (int x = 0; x < 2; ++x)
{
    samples[ x ] = LinearizeDepth(gDepthTexture.Load(int3(In.Position.xy + offsets[ x ] * 2, 0)).x);
}

for (int x = 2; x < 4; ++x)
{
    samples[ x ] = LinearizeDepth(gDepthTexture.Load(int3(In.Position.xy + offsets[ x ] * 2, 0)).x);
}

float widthMax = max(abs(samples[ 0 ] - currentPixel), abs(samples[ 1 ] - currentPixel));
float heightMax = max(abs(samples[ 2 ] - currentPixel), abs(samples[ 2 ] - currentPixel));

float finalMax = max(widthMax, heightMax);

// ID INFO

int IDs[ 4 ];
for (int x = 0; x < 2; ++x)
{
    IDs[ x ] = GetIDFromTex(gIDTex.Load(int3(In.Position.xy + offsets[ x ] * 2, 0)));
}

for (int x = 2; x < 4; ++x)
{
    IDs[ x ] = GetIDFromTex(gIDTex.Load(int3(In.Position.xy + offsets[ x ] * 2, 0)));
}

// if id matches
if (IDs[ 0 ] != IDs[ 1 ])
{
    // if object is close enough, forgive them

    if (widthMax < gLightStep / 10000.0f)
        return float4(0, 0, 0, 0);

    return float4(0, 0, 0, 1);
}

if (IDs[ 2 ] != IDs[ 3 ])
{
    if (heightMax < gLightStep / 10000.0f)
        return float4(0, 0, 0, 0);

    return float4(0, 0, 0, 1);
}


return float4(0, 0, 0, 0);
}