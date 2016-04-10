/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShaderBufferList.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <DirectXMath.h>
#include "Particle.h"

namespace ursine
{
    namespace graphics
    {
        enum COMPUTE_BUFFER_LIST : unsigned
        {
            COMPUTE_BUFFER_ID = 0,
            COMPUTE_BUFFER_ID_CPU,
            COMPUTE_BUFFER_MOUSEPOS,
            COMPUTE_BUFFER_COUNT
        };

        enum BUFFER_LIST : unsigned
        {
            BUFFER_CAMERA = 0,
            BUFFER_TRANSFORM,
            BUFFER_DIRECTIONAL_LIGHT,
            BUFFER_POINT_LIGHT,
            BUFFER_INV_PROJ,
            BUFFER_PRIM_COLOR,
            BUFFER_POINT_GEOM,
            BUFFER_BILLBOARDSPRITE,
            BUFFER_GBUFFER_UNPACK,
            BUFFER_LIGHT_PROJ,
            BUFFER_MATERIAL_DATA,
            BUFFER_SPOTLIGHT,
            BUFFER_MATRIX_PAL,
            BUFFER_MOUSEPOS,
            BUFFER_PARTICLEDATA,
            BUFFER_SHADOWMAP,
            BUFFER_TEX_OFFSET,
            BUFFER_LIGHT_FALLOFF,

            BUFFER_FRAG_VS,
            BUFFER_FRAG_GS,
            BUFFER_FRAG_PS,

            // non-default
            BUFFER_GLYPHDATA,   // 6
            BUFFER_TEXTDATA,    // 7

            BUFFER_COUNT,

            //"virtual" buffer
        };

        struct CameraBuffer
        {
            DirectX::XMMATRIX view;
            DirectX::XMMATRIX projection;
        };

        struct TransformBuffer
        {
            DirectX::XMMATRIX transform;
        };

        struct DirectionalLightBuffer
        {
            DirectX::XMFLOAT4 lightDirection;
            DirectX::XMFLOAT3 lightColor;
            float intensity;
            DirectX::XMFLOAT4 lightPosition;
        };

        struct invViewBuffer
        {
            DirectX::XMMATRIX invView;
            float nearPlane;
            float farPlane;
        };

        struct PointLightBuffer
        {
            DirectX::XMFLOAT3 lightPos;
            float lightRadius;
            DirectX::XMFLOAT3 color;
            float intensity;
        };

        struct PrimitiveColorBuffer
        {
            DirectX::XMFLOAT4 color;
        };

        struct PointGeometryBuffer
        {
            DirectX::XMFLOAT4 cameraPosition;
            DirectX::XMFLOAT4 cameraUp;
        };

        struct BillboardSpriteBuffer
        {
            float width;
            float height;
            DirectX::XMFLOAT2 buffer;
        };

        struct GBufferUnpackBuffer
        {
            DirectX::XMFLOAT4 perspectiveVals;
            DirectX::XMFLOAT4 x4ViewInv;
        };

        struct MaterialDataBuffer
        {
            float emissive;
            float specularPower;
            float specularIntensity;
            unsigned id;
        };

        struct SpotlightBuffer
        {
            DirectX::XMFLOAT3 lightPosition;
            float intensity;
            DirectX::XMFLOAT3 lightDirection;
            float innerAngle;
            DirectX::XMFLOAT3 diffuseColor;
            float outerAngle;
            DirectX::XMFLOAT3 falloffValues;
            float lightSize;
        };

        struct MatrixPalette
        {
            DirectX::XMMATRIX matPal[ 96 ];
        };

        struct MatrixPalBuffer
        {
            MatrixPalette matPal;
            //MatrixPalette matPalIT;
        };

        struct MouseBuffer
        {
            DirectX::XMINT4 mousePos;
        };

        struct ComputeIDOutput
        {
            unsigned id;
            unsigned x;
            unsigned y;
            float depth;
        };

        struct ParticleBuffer
        {
            Particle_GPU data[ 1024 ];
        };

        struct Glyph
        {
            DirectX::XMFLOAT2 screenPosition;
            DirectX::XMFLOAT2 glyphPosition;
            DirectX::XMFLOAT2 glyphSize;
            DirectX::XMFLOAT2 buffer;
        };

        struct GlyphBuffer
        {
            Glyph glyphData[ 1024 ];
        };

        struct SpriteTextBuffer
        {
            DirectX::XMFLOAT3 worldPosition;
            float offset;

            DirectX::XMFLOAT2 sizeScalar;
            DirectX::XMFLOAT2 textureDimensions;
        };

        struct ShadowProjectionBuffer
        {
            DirectX::XMMATRIX invCam;
            DirectX::XMMATRIX lightView;
            DirectX::XMMATRIX lightProj;
        };

        struct TextureUVOffset
        {
            DirectX::XMFLOAT2 uv;
            DirectX::XMFLOAT2 buffer;
        };

        struct FalloffBuffer
        {
            float lightSteps;
            float borderCutoff;
            float farDistance;
            float buffer;
        };

        struct FragmentationVSBuffer
        {
            // normal offset scalar
            float normalOffset;
            DirectX::XMFLOAT3 buffer;
        };

        struct FragmentationGSBuffer
        {
            // vertical force value
            float verticalForce;

            // horizontal force value
            float horizontalForce;

            // outward force value
            float outwardForce;

            // gravity force value
            float gravityForce;

            // time
            float time;

            DirectX::XMFLOAT3 buffer;
        };

        struct FragmentationPSBuffer
        {
            // pulse rate
            float pulseSpeed;

            // fade
            float fadeAmount;

            // time
            float time;

            // start/end time
            float startTime;
            float endTime;

            // normal transparency
            float transparencyThreshold;

            float globalTime;

            float buffer;

            DirectX::XMFLOAT4 color;
        };
    }
}
