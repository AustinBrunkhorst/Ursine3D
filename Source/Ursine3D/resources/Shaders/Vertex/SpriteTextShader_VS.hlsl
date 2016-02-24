cbuffer CameraBuffer : register(b0)
{
    matrix viewMatrix;
    matrix projection;
};

cbuffer invView : register(b4)
{
    matrix invView;
    float nearPlane;
    float farPlane;
};

cbuffer posAndColor : register(b6)
{
    float4 cameraPosition;
    float4 color;
}

cbuffer TextData : register(b7)
{
    // boundry ////////////
    // position of this piece of text
    float3 textPosition;        // position of this piece of text
    float offset;               // total length of the text

    // boundry ////////////
    float2 sizeScalar;          // size scalars for width and height
    float2 textureDimensions;   // overall size of the texture
}

struct GlyphData
{
    // alignment is really, really important
    // boundry ////////////
    float2 position;        // position w/ respect to start of word in screen space
    float2 glyphPosition;     // starting position of this glyph

    // boundry ////////////
    float2 glyphSize;       // width of this glyph
    float2 dimensions;
};

cbuffer GlyphDataBuffer : register(b10)
{
    GlyphData g_glyphData[ 1024 ];
}

// dummy for reflection data
struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
};

float2x2 GenerateRotation(float radianAngle)
{
    return float2x2(float2(cos(radianAngle), -sin(radianAngle)), (float2(sin(radianAngle), cos(radianAngle))));
}

PixelInputType main(uint id : SV_VERTEXID)
{
    PixelInputType output;

    uint glyphIndex = (id) / 6;
    uint vertexInQuad = (id) % 6;

    // generate UV
    float2 uvStart = g_glyphData[ glyphIndex ].glyphPosition;
    float2 uvScalar = g_glyphData[ glyphIndex ].glyphSize;

    output.uv = uvStart;

    float3 position;
    switch ( vertexInQuad )
    {
    case 0:
        position.x = 0;
        position.y = 1;
        position.z = 0.f;

        output.uv.x += 0;
        output.uv.y += 0;
        break;
    case 1:
        position.x = 1;
        position.y = 1;
        position.z = 0.f;

        output.uv.x += 1 * uvScalar.x;
        output.uv.y += 0;
        break;
    case 2:
        position.x = 0;
        position.y = 0;
        position.z = 0.f;

        output.uv.x += 0;
        output.uv.y += 1 * uvScalar.y;
        break;
    case 3:
        position.x = 1;
        position.y = 1;
        position.z = 0.f;

        output.uv.x += 1 * uvScalar.x;
        output.uv.y += 0;
        break;
    case 4:
        position.x = 1;
        position.y = 0;
        position.z = 0.f;

        output.uv.x += 1 * uvScalar.x;
        output.uv.y += 1 * uvScalar.y;
        break;
    default:
        position.x = 0;
        position.y = 0;
        position.z = 0.f;

        output.uv.x += 0;
        output.uv.y += 1 * uvScalar.y;
        break;
    }

    // scale to right dimensions for this letter, scaling this polygon to match the size of the glyph
    position.xy *= uvScalar * sizeScalar;

    // translate glyph in view space
    position = float3(position + float3( (g_glyphData[ id / 6 ].position ) * sizeScalar, 0));

    // transform into world w/ inv view
    float3 finalPosition = mul(position, (float3x3)invView);

    // translate to world position
    finalPosition += textPosition;

    // move into view space
    output.position = mul(float4(finalPosition, 1.0f), viewMatrix);

    // move into screen space
    output.position = mul(output.position, projection);
    output.color = color;

    return output;
}