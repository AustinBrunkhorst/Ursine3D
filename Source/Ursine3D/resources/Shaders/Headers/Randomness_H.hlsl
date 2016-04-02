float random(float2 position, float seed)
{
    float fixedSeed = abs(seed) + 1.0;
    float x = dot(position, float2(12.9898, 78.233) * fixedSeed);
    return frac(sin(x) * 43758.5453);
}

float2 f2RandomHash(float2 seed)
{
    return float2(random(seed, 2.0),
        random(seed, 3.0));
}

float3 f3RandomHash(float2 seed)
{
    return float3(random(seed, 2.0),
        random(seed, 3.0),
        random(seed, 4.0));
}

float4 f4RandomHash(float2 seed)
{
    return float4(random(seed, 2.0),
        random(seed, 3.0),
        random(seed, 4.0),
        random(seed, 6.0));
}

float4 DitherRGBA(float4 color, float2 seed)
{
    return color + f4RandomHash(seed) / 255.0; //8 bit
}

float2 DitherRG(float2 color, float2 seed)
{
    return color + f2RandomHash(seed) / 255.0; //8 bit
}

float3 DitherRGB(float3 color, float2 seed)
{
    return color + f3RandomHash(seed) / 255.0; //8 bit
}

float3 DitherNormal(float3 normal, float2 seed)
{
    return normal + f3RandomHash(seed) / 255.0; //16 bit
}
