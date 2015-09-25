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
    void Initialize( ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath );
    void Uninitialize( );

    void MapTextureByName( std::string name, unsigned int bufferIndex = 0 );
    void MapTextureByID( unsigned ID, unsigned int bufferIndex = 0 );

    void MapSamplerState( Sampler type, unsigned bufferIndex = 0 );

    unsigned GetTextureIDByName( std::string name );

  private:
    void TextureLoadBackend( std::string name, std::string path, unsigned width, unsigned height );

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