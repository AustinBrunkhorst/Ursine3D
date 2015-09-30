/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      TextureManager.h
Module:         Graphics
Purpose:        class for managing textures
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>
#include <map>
#include <string>
#include <d3d11.h>
#include "Texture.h"
#include "SamplerList.h"
#include "GraphicsDefines.h"

namespace ursine
{
    class TextureManager
    {
    public:
        void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath);
        void Uninitialize(void);

        void MapTextureByName(const std::string name, const unsigned int bufferIndex = 0);
        void MapTextureByID( const unsigned ID, const unsigned int bufferIndex = 0);

        void MapSamplerState( const Sampler type, const unsigned bufferIndex = 0);

        unsigned GetTextureIDByName( const std::string name);

    private:
        void TextureLoadBackend( const std::string name, const std::string path, const unsigned width, const unsigned height);

    private:
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_deviceContext;

        unsigned m_textureCount;

        std::map<std::string, Texture*> m_textureList;
        std::map<unsigned int, Texture*> m_hashTextureList;
        std::map<std::string, unsigned int> m_lookupTextureList;

        std::vector<ID3D11SamplerState*> m_samplerStateList_;
    };
}
