/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VertexDefinitions.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      VertexDefinitions.h
Module:         Graphics
Purpose:        definitions of vertices
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <DirectXMath.h>

namespace ursine
{
    namespace graphics
    {
        enum VERTEX_TYPES
        {
            VERTEX_BASIC = 0,
            VERTEX_DIFFUSE,
            VERTEX_DRAWING,
			VERTEX_ANIMATION,
            VERTEX_COUNT
        };

        struct BasicVertex
        {
            DirectX::XMFLOAT4 pos;
        };

        struct DiffuseVertex
        {
            DirectX::XMFLOAT4 pos;
            DirectX::XMFLOAT4 normal;
        };

        struct DiffuseTextureVertex
        {
            DirectX::XMFLOAT4 pos;
            DirectX::XMFLOAT4 normal;
            DirectX::XMFLOAT2 UV;

            //@matt don't forget this
        };

		struct AnimationVertex
		{
			DirectX::XMFLOAT3	vPos;
			DirectX::XMFLOAT3	vNor;
			DirectX::XMFLOAT2	vUv;
			DirectX::XMFLOAT4	vBWeight;
			BYTE				vBIdx[4];
		};

        struct PrimitiveVertex
        {
            DirectX::XMFLOAT4 pos;
            DirectX::XMFLOAT4 color;
            float size;
        };
    }
}
