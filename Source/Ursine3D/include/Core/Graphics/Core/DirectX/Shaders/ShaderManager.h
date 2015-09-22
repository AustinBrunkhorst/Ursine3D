/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ShaderManager.h
Module:         Graphics
Purpose:        backend manager for shader management
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "D3D11Forward.h"
#include <string>
#include <vector>

#include "ShaderTypes.h"

namespace ursine
{
  namespace DXCore
  {
    struct Shader
    {
      SHADER_TYPES type;

      ID3D11VertexShader *vs;
      ID3D11PixelShader *ps;
      ID3D11HullShader *hs;
      ID3D11DomainShader *ds;
      ID3D11GeometryShader *gs;

      ID3D11ShaderReflection *reflectionData;
      ID3DBlob *vsBlob;
      void *rawData;
      unsigned size;

      std::vector<std::string> filenames;
      std::vector<time_t> timestamps;

      Shader( )
      {
        filenames.resize( DEF_COUNT );
        timestamps.resize( DEF_COUNT );
      }
    };

    class ShaderManager
    {
    public:
      void Initialize( ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath = "" );
      void Uninitialize( );

      void BindShader( SHADER_TYPES shader );

      void LoadShader( SHADER_TYPES shader, const char *filename );

      void Render( unsigned vert_count );

      Shader *GetShader( SHADER_TYPES shader );
    private:
      std::string m_shaderPath;
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_deviceContext;

      SHADER_TYPES m_currentState;
      std::vector<Shader*> m_shaderArray;
    };
  }
}