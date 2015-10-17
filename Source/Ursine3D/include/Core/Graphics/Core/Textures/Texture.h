/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      Texture.h
Module:         Graphics
Purpose:        Texture class
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"

namespace ursine
{
    namespace graphics
    {
        struct Texture
        {
            Texture() : m_width(0), m_height(0), m_shaderResource(nullptr), m_texture2d(nullptr) {}
            unsigned m_width;
            unsigned m_height;
            ID3D11ShaderResourceView *m_shaderResource;
            ID3D11Texture2D *m_texture2d;
        };
    }
}
