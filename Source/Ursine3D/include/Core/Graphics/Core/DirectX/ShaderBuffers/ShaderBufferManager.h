/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShaderBufferManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
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
#include "GfxDefines.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            class ShaderBufferManager
            {
            public:
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *context);
                void Uninitialize(void);

                //buffer mapping
                void MapCameraBuffer(const SMat4 &view, const SMat4 &projection, SHADERTYPE shader = SHADERTYPE_VERTEX, unsigned int bufferIndex = BUFFER_CAMERA);
                void MapTransformBuffer(const SMat4 &transform, const SHADERTYPE shader = SHADERTYPE_VERTEX, unsigned int bufferIndex = BUFFER_TRANSFORM);

                //map any buffer to the GPU
                template<BUFFER_LIST buffer, typename T>
                void MapBuffer(const T *data, const SHADERTYPE shader, const unsigned int bufferIndex = buffer)
                {
                    UAssert(bufferIndex < MAX_CONST_BUFF, "ResourceManager attempted to map buffer to invalid index (index #%i)", bufferIndex);
                    HRESULT result;
                    D3D11_MAPPED_SUBRESOURCE mappedResource;

                    T *dataPtr;

                    //make sure buffer exists
                    UAssert(m_bufferArray[ buffer ] != nullptr, "A buffer was never initialized!");

                    //lock the buffer
                    result = m_deviceContext->Map(m_bufferArray[ buffer ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

                    UAssert(result == S_OK, "Failed to lock buffer for writing!");

                    //grab data
                    dataPtr = reinterpret_cast<T*>(mappedResource.pData);

                    //set data
                    memcpy(dataPtr, data, sizeof(T));

                    //unlock buffer
                    m_deviceContext->Unmap(m_bufferArray[ buffer ], 0);

                    //map to a given buffer
                    SetBuffer(shader, bufferIndex, m_bufferArray[ buffer ]);
                }

                //map compute buffer to the GPU
                template<COMPUTE_BUFFER_LIST buffer, typename T>
                void MapComputeBuffer(const T *data, const SHADERTYPE shader, const unsigned int bufferIndex = buffer)
                {
                    UAssert(bufferIndex < MAX_CONST_BUFF, "ResourceManager attempted to map buffer to invalid index (index #%i)", bufferIndex);
                    HRESULT result;
                    D3D11_MAPPED_SUBRESOURCE mappedResource;

                    T *dataPtr;

                    //make sure buffer exists
                    UAssert(m_computeBufferArray[ buffer ] != nullptr, "A buffer was never initialized!");

                    //lock the buffer
                    result = m_deviceContext->Map(m_computeBufferArray[ buffer ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                    UAssert(result == S_OK, "Failed to map buffer!");

                    //grab data
                    dataPtr = reinterpret_cast<T*>(mappedResource.pData);

                    //set data
                    memcpy(dataPtr, data, sizeof(T));

                    //unlock buffer
                    m_deviceContext->Unmap(m_computeBufferArray[ buffer ], 0);

                    //map to a given buffer
                    SetBuffer(shader, bufferIndex, m_computeBufferArray[ buffer ]);
                }

                //read data from the cpu
                template<COMPUTE_BUFFER_LIST buffer, typename T>
                void ReadComputeBuffer(T *data, const SHADERTYPE shader, const unsigned int bufferIndex = buffer)
                {
                    UAssert(bufferIndex < MAX_CONST_BUFF, "ResourceManager attempted to map buffer to invalid index (index #%i)", bufferIndex);
                    HRESULT result;
                    D3D11_MAPPED_SUBRESOURCE mappedResource;

                    T *dataPtr;

                    //make sure buffer exists
                    UAssert(m_computeBufferArray[ buffer ] != nullptr, "A buffer was never initialized!");

                    //lock the buffer for reading
                    result = m_deviceContext->Map(m_computeBufferArray[ buffer ], 0, D3D11_MAP_READ, 0, &mappedResource);
                    UAssert(result == S_OK, "Failed to map buffer!");

                    //grab data
                    dataPtr = reinterpret_cast<T*>(mappedResource.pData);

                    //copy data into the target
                    memcpy(data, dataPtr, sizeof(T));

                    //unlock buffer
                    m_deviceContext->Unmap(m_computeBufferArray[ buffer ], 0);
                }
            
                // @matt temp /////////////////////////////////////////////////////////////////////
                //compute data
                std::vector<ID3D11Buffer *> m_computeBufferArray;
                std::vector<ID3D11ShaderResourceView *> m_computeSRV;
                std::vector<ID3D11UnorderedAccessView *> m_computeUAV;
            private:

                //sets the buffer for a given shader
                void SetBuffer(const SHADERTYPE shader, const unsigned bufferIndex, ID3D11Buffer *buffer);

                //makes the buffers for mapping resources
                template<typename T>                    //D3D11_USAGE_DYNAMIC           D3D11_CPU_ACCESS_WRITE
                void MakeBuffer(const BUFFER_LIST type, unsigned gpuUsage = 2, unsigned cpuAccess = 0x10000L);

                //create compute buffer
                template<typename T>
                void MakeComputeBuffer(const int count, const COMPUTE_BUFFER_LIST type, unsigned usageFlag, unsigned bindFlags, unsigned cpuAccess = 0x10000L, bool bufferStruct2D = true);
                
                //create SRV
                void MakeComputeSRV(const int count, const COMPUTE_BUFFER_LIST type);

                //create UAV
                void MakeComputeUAV(const int count, const COMPUTE_BUFFER_LIST type);

                //members
                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;

                std::vector<ID3D11Buffer *> m_bufferArray;


            };
        }
    }
}
