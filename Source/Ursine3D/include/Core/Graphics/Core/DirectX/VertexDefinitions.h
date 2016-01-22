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
