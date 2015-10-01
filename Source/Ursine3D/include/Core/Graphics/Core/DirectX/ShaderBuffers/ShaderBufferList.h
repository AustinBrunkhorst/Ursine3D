/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ShaderBufferList.h
Module:         Graphics
Purpose:        Enum of buffers + their definitions
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <DirectXMath.h>

enum BUFFER_LIST
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

struct InvProjBuffer
{
    DirectX::XMMATRIX invProj;
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
    float buffer;
};
