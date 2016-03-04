/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Texture.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"

namespace ursine
{
    namespace graphics
    {
        struct Texture
        {
            Texture() 
                : m_width( 0 ), 
                m_height( 0 ), 
                m_internalID( -1 ),
                m_referenceCount( 0 ),
                m_shaderResource( nullptr ), 
                m_texture2d( nullptr )
            {
            }

            unsigned m_width;
            unsigned m_height;
            unsigned m_internalID;

            unsigned long long m_referenceCount;

            ID3D11ShaderResourceView *m_shaderResource;
            ID3D11Texture2D *m_texture2d;

            uint8_t *m_binaryData;
            size_t m_binarySize;
        };
    }
}
