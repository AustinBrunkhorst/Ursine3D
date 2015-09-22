/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ShaderBufferManager.h
Module:         Graphics
Purpose:        Class for managing buffers
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"
#include <vector>

#include "ShaderTypes.h"
#include "ShaderBufferList.h"
#include "GraphicsDefines.h"

namespace ursine
{
  namespace DXCore
  {
    class ShaderBufferManager
    {
    public:
      void Initialize( ID3D11Device *device, ID3D11DeviceContext *context );
      void Uninitialize( );

      //buffer mapping
      void MapCameraBuffer(const SMat4 &view, const SMat4 &projection, SHADERDEF shader = VERTEX_SHADER, unsigned int bufferIndex = BUFFER_CAMERA );
      void MapTransformBuffer(const SMat4 &transform, SHADERDEF shader = VERTEX_SHADER, unsigned int bufferIndex = BUFFER_TRANSFORM );

      template<BUFFER_LIST buffer, typename T>
      void MapBuffer( T *data, SHADERDEF shader, unsigned int bufferIndex = buffer )
      {
        UAssert( bufferIndex < MAX_CONST_BUFF, "ResourceManager attempted to map buffer to invalid index (index #%i)", bufferIndex );
        HRESULT result;
        D3D11_MAPPED_SUBRESOURCE mappedResource;

        T *dataPtr;

        //make sure buffer exists
        UAssert( m_bufferArray[ buffer ] != NULL, "A buffer was never initialized!" );

        //lock the buffer
        result = m_deviceContext->Map( m_bufferArray[ buffer ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

        //grab data
        dataPtr = (T*)mappedResource.pData;

        //set data
        memcpy( dataPtr, data, sizeof( T ) );

        //unlock buffer
        m_deviceContext->Unmap( m_bufferArray[ buffer ], 0 );

        //map to a given buffer
        SetBuffer( shader, bufferIndex, m_bufferArray[ buffer ] );
      }
    private:

      //sets the buffer for a given shader
      void SetBuffer( SHADERDEF shader, unsigned bufferIndex, ID3D11Buffer *buffer );

      //makes the buffers for mapping resources
      template <typename T>
      void MakeBuffer( BUFFER_LIST type );

      //members
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_deviceContext;

      std::vector<ID3D11Buffer *> m_bufferArray;
    };
  }
}