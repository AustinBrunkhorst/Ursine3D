// depth texture
Texture2D    gDepthTexture          : register(t0);
Texture2D    gIDTex          : register(t1);

// sample state
SamplerState SampleType             : register(s0);

//inv projection 
cbuffer InverseProjection           : register(b4)
{
    float4x4    gInvProj;
    float       gAValue;
    float       gBValue;
};

cbuffer ShadowFalloff               : register(b12)
{
    float gLightStep;
    float gBorderCutoff;
    float gFarDistance;
    float buffer;
}

// convert given depth value from exponential to linear
float LinearizeDepth(float depth)
{
    return gBValue / (depth - gAValue);
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
    // top left is 0, 0 on screen

    float2 offsets[ 8 ] = {
        { -1,-1 }, // tl
        { 0, -1 }, // top
        { 1, -1 }, // tr
        { -1, 0 }, // l
                   // center not used
        { 1,  0 }, // right
        { -1,  1 }, // bl
        { 0,  1 }, // bottom
        { 1,  1 } // bottom right
    };

    float weights[8] = 
    {
        2,
        2, 
        2,
        2,
        2,
        2,
        2,
        2
    };

    float samples[8];

    float base = LinearizeDepth(gDepthTexture.Load(int3(In.Position.xy, 0)).x);

    // first, to save sampling we sample the 8 pixels surrounding base pixel
    for (int x = 0; x < 8; ++x)
    {
        samples[x] = LinearizeDepth(gDepthTexture.Load(int3(In.Position.xy + offsets[ x ] * weights[x], 0)).x) ;
    }

    // Sobel operator is defined as the magnitude of the gradient sampled by the masks, 
    // M = sqrt( xMask^2 + yMask^2 )

    // 0 1 2
    // 3 x 4 
    // 5 6 7

    // the x mask is:
    // -1 0 +1
    // -2 0 +2
    // -1 0 +1
    float sX = -samples[0] + samples[2] - 2 * samples[3] + 2 * samples[4] - samples[5] + samples[7];

    // y mask is rotated 90 degrees
    float sY = samples[0] + 2 * samples[1] + samples[2] - (samples[5] + 2 * samples[6] + samples[7]);

    float mag = (sqrt(sX * sX + sY * sY) - (base * base * base) / (gLightStep * 10000))/ 2500;

    //return float4(mag, mag, mag, 1);

    //// must beat 14% of max distance
    //if (mag > (2500 + base * base / gLightStep) * 0.01 * 14)
    //    return float4(0, 0, 0, 1);

    /////////////////////////////////////////////////////////////////
    // LAPLACIAN GAUSSIAN

    // mask is as follows:
    // 1 4 1
    // 4 -20 4
    // 1 4 1

    float sides = 1;
    float corners = 0;
    float main = -4;

    float lapGauss = 
        main * base + 
        samples[0] * corners + samples[ 2 ] * corners + samples[ 5 ] * corners + samples[ 7 ] * corners +
        samples[ 1 ] * sides + samples[ 3 ] * sides + samples[ 4 ] * sides + samples[ 6 ] * sides;

    lapGauss /= gFarDistance;

    if (lapGauss > 0.07)
        return float4(0, 0, 0, 1);

    return float4(0, 0, 0, 0);
}