 
//texture
Texture2D modelTexture : register(t0);

//sample type
SamplerState texSample : register(s0);

cbuffer camInfo : register(b6)
{
  float4 camPos;
  float4 camLook;
};

cbuffer PrimColorBuffer : register(b5)
{
    float4 color;
}

cbuffer MaterialBuffer : register(b10)
{
    float emissive;
    float specularPower;
    float specularIntensity;
    float buffer;
};

//what incoming pixel data looks like
struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : UV;
};

float4 main(PixelInputType input) : SV_TARGET
{
  float4 diffuseColor = float4(1, 1, 1, 1);
  float4 downColor = float4(1, 1, 1, 1);
  float4 upColor = float4(0.2, 0.2, 0.2, 1);


  float4 textureColor;
  float4 lightDir;
  float upIntensity, downIntensity;
  float4 color;

  textureColor = modelTexture.Sample( texSample, input.uv);

  upIntensity = (dot(input.normal, float4(0, 1, 0, 0)));
  downIntensity = (dot( input.normal, float4(0, -1, 0, 0) ));


  // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
  color = (textureColor * 0.5f + textureColor * ((clamp( downIntensity, 0, 1 ) * upColor) + (downColor * clamp( upIntensity, 0, 1 )))) * (1 - emissive) + textureColor * emissive;
  color.w = 1;

  return color;
}