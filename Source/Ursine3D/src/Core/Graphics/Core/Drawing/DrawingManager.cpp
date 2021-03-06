/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DrawingManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "VertexDefinitions.h"
#include "DrawingManager.h"
#include "GfxDefines.h"
#include <d3d11.h>

namespace ursine
{
    namespace graphics
    {
        void DrawingManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devCon)
        {
            m_device = device;
            m_deviceContext = devCon;

            m_pointList.resize(MAX_DRAW_OBJ);
            m_lineList.resize(MAX_DRAW_OBJ);
            m_size = 1;

            m_pointCount = 0;
            m_lineCount = 0;

            m_pointListOverdraw.resize(MAX_DRAW_OBJ);
            m_lineListOverdraw.resize(MAX_DRAW_OBJ);

            m_pointCountOverdraw = 0;
            m_lineCountOverdraw = 0;
            m_overdraw = false;

            //create the buffers
            // CREATE VERTEX BUFFER /////////////////////////////////////////
            D3D11_BUFFER_DESC vertexBufferDesc;
            D3D11_SUBRESOURCE_DATA vertexData;
            HRESULT result;

            //Set up the description of the static vertex buffer.
            vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            vertexBufferDesc.ByteWidth = sizeof(PrimitiveVertex) * MAX_DRAW_OBJ;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            vertexBufferDesc.MiscFlags = 0;
            vertexBufferDesc.StructureByteStride = 0;

            //Give the subresource structure a pointer to the vertex data.
            vertexData.pSysMem = &m_pointList[ 0 ];
            vertexData.SysMemPitch = 0;
            vertexData.SysMemSlicePitch = 0;

            //Now create the vertex buffer.
            result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertPointBuffer);
            UAssert(result == S_OK, "Failed to make vertex buffer!");

            vertexData.pSysMem = &m_pointListOverdraw[ 0 ];
            result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertPointBufferOverdraw);
            UAssert(result == S_OK, "Failed to make vertex buffer!");

            // CREATE VERTEX BUFFER /////////////////////////////////////////
            D3D11_BUFFER_DESC vertexBufferDesc2;
            D3D11_SUBRESOURCE_DATA vertexData2;

            //Set up the description of the static vertex buffer.
            vertexBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
            vertexBufferDesc2.ByteWidth = sizeof(PrimitiveVertex) * MAX_DRAW_OBJ;
            vertexBufferDesc2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            vertexBufferDesc2.MiscFlags = 0;
            vertexBufferDesc2.StructureByteStride = 0;

            //Give the subresource structure a pointer to the vertex data.
            vertexData2.pSysMem = &m_lineList[ 0 ];
            vertexData2.SysMemPitch = 0;
            vertexData2.SysMemSlicePitch = 0;

            //Now create the vertex buffer.
            result = m_device->CreateBuffer(&vertexBufferDesc2, &vertexData2, &m_vertLineBuffer);
            UAssert(result == S_OK, "Failed to make vertex buffer!");

            vertexData2.pSysMem = &m_lineListOverdraw[ 0 ];
            result = m_device->CreateBuffer(&vertexBufferDesc2, &vertexData2, &m_vertLineBufferOverdraw);
            UAssert(result == S_OK, "Failed to make vertex buffer!");

            // CREATE INDEX BUFFER //////////////////////////////////////////

            unsigned *indexArray = new unsigned[ MAX_DRAW_OBJ ];

            for (unsigned x = 0; x < MAX_DRAW_OBJ; ++x)
                indexArray[ x ] = x;

            D3D11_BUFFER_DESC indexBufferDesc;
            D3D11_SUBRESOURCE_DATA indexData;

            //Set up the description of the static index buffer.
            indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            indexBufferDesc.ByteWidth = sizeof(unsigned) * MAX_DRAW_OBJ;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;
            indexBufferDesc.StructureByteStride = 0;

            //Give the subresource structure a pointer to the index data.
            indexData.pSysMem = indexArray;
            indexData.SysMemPitch = 0;
            indexData.SysMemSlicePitch = 0;

            //Create the index buffer.
            result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
            result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBufferOverdraw);
            UAssert(result == S_OK, "Failed to make index buffer!");
        }

        void DrawingManager::Uninitialize(void)
        {
            RELEASE_RESOURCE(m_vertPointBuffer);
            RELEASE_RESOURCE(m_vertLineBuffer);
            RELEASE_RESOURCE(m_indexBuffer);

            RELEASE_RESOURCE(m_vertPointBufferOverdraw);
            RELEASE_RESOURCE(m_vertLineBufferOverdraw);
            RELEASE_RESOURCE(m_indexBufferOverdraw);
            m_device = nullptr;
            m_deviceContext = nullptr;
        }

        void DrawingManager::EndScene(void)
        {
            m_lineCount = 0;
            m_pointCount = 0;

            m_lineCountOverdraw = false;
            m_pointCountOverdraw = false;
        }

        void DrawingManager::SetDrawColor(float x, float y, float z, float a)
        {
            m_color = Color(x, y, z, a);
        }

        void DrawingManager::SetDrawColor(const Color &color)
        {
            m_color = color;
        }
        Color &DrawingManager::GetColor(void)
        {
            return m_color;
        }

        void DrawingManager::SetSize(float width)
        {
            m_size = width;
        }

        float DrawingManager::GetSize(void)
        {
            return m_size;
        }

        void DrawingManager::SetOverdraw(bool draw)
        {
            m_overdraw = draw;
        }

        void DrawingManager::DrawPoint(float x, float y, float z)
        {
            if (!m_overdraw)
            {
                UAssert(m_pointCount < MAX_DRAW_OBJ, "No more room for drawable objects! Let Matt now, he'll fix it");

                m_pointList[ m_pointCount ].pos = DirectX::XMFLOAT4(x, y, z, 1);
                m_pointList[ m_pointCount ].size = m_size;
                m_pointList[ m_pointCount++ ].color = DirectX::XMFLOAT4(m_color.ToVector4( ).GetFloatPtr( ));
            }
            else
            {
                UAssert(m_pointCountOverdraw < MAX_DRAW_OBJ, "No more room for drawable objects! Let Matt now, he'll fix it");

                m_pointListOverdraw[ m_pointCountOverdraw ].pos = DirectX::XMFLOAT4(x, y, z, 1);
                m_pointListOverdraw[ m_pointCountOverdraw ].size = m_size;
                m_pointListOverdraw[ m_pointCountOverdraw++ ].color = DirectX::XMFLOAT4(m_color.ToVector4( ).GetFloatPtr( ));
            }
        }

        void DrawingManager::DrawLine(float x0, float y0, float z0, float x1, float y1, float z1)
        {
            if (!m_overdraw)
            {
                UAssert(m_lineCount < MAX_DRAW_OBJ, "No more room for drawable objects! Let Matt now, he'll fix it");
                m_lineList[ m_lineCount ].pos = DirectX::XMFLOAT4(x0, y0, z0, 1);
                m_lineList[ m_lineCount ].size = m_size;
                m_lineList[ m_lineCount++ ].color = DirectX::XMFLOAT4(m_color.ToVector4( ).GetFloatPtr( ));

                m_lineList[ m_lineCount ].pos = DirectX::XMFLOAT4(x1, y1, z1, 1);
                m_lineList[ m_lineCount ].size = m_size;
                m_lineList[ m_lineCount++ ].color = DirectX::XMFLOAT4(m_color.ToVector4( ).GetFloatPtr( ));
            }
            else
            {
                UAssert(m_lineCountOverdraw < MAX_DRAW_OBJ, "No more room for drawable objects! Let Matt now, he'll fix it");
                m_lineListOverdraw[ m_lineCountOverdraw ].pos = DirectX::XMFLOAT4(x0, y0, z0, 1);
                m_lineListOverdraw[ m_lineCountOverdraw ].size = m_size;
                m_lineListOverdraw[ m_lineCountOverdraw++ ].color = DirectX::XMFLOAT4(m_color.ToVector4( ).GetFloatPtr( ));

                m_lineListOverdraw[ m_lineCountOverdraw ].pos = DirectX::XMFLOAT4(x1, y1, z1, 1);
                m_lineListOverdraw[ m_lineCountOverdraw ].size = m_size;
                m_lineListOverdraw[ m_lineCountOverdraw++ ].color = DirectX::XMFLOAT4(m_color.ToVector4( ).GetFloatPtr( ));
            }
        }

        void DrawingManager::ConstructPointMesh(unsigned &vertCount, unsigned &indexCount, ID3D11Buffer **mesh, ID3D11Buffer **indices)
        {
            vertCount = m_pointCount;

            // UPDATE VERTEX BUFFER /////////////////////////////////////////
            HRESULT result;
            D3D11_MAPPED_SUBRESOURCE mappedResource;

            //buffer of the data
            PrimitiveVertex *buffer;

            //lock the buffer
            result = m_deviceContext->Map(m_vertPointBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            //get the buffer
            buffer = (PrimitiveVertex*)mappedResource.pData;

            //copy the data over
            memcpy(buffer, &m_pointList[ 0 ], sizeof(PrimitiveVertex) * vertCount);

            //unlock
            m_deviceContext->Unmap(m_vertPointBuffer, 0);

            indexCount = vertCount;
            *indices = m_indexBuffer;
            *mesh = m_vertPointBuffer;
        }

        void DrawingManager::ConstructLineMesh(unsigned &vertCount, unsigned &indexCount, ID3D11Buffer **mesh, ID3D11Buffer **indices)
        {
            vertCount = m_lineCount;

            // UPDATE VERTEX BUFFER /////////////////////////////////////////
            HRESULT result;
            D3D11_MAPPED_SUBRESOURCE mappedResource;

            //buffer of the data
            PrimitiveVertex *buffer;

            //lock the buffer
            result = m_deviceContext->Map(m_vertLineBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            //get the buffer
            buffer = (PrimitiveVertex*)mappedResource.pData;

            //copy the data over
            memcpy(buffer, &m_lineList[ 0 ], sizeof(PrimitiveVertex) * vertCount);

            //unlock
            m_deviceContext->Unmap(m_vertLineBuffer, 0);

            indexCount = vertCount;
            *indices = m_indexBuffer;
            *mesh = m_vertLineBuffer;
        }

        bool DrawingManager::CheckRenderPoints(void) const
        {
            return m_pointCount > 0;
        }

        bool DrawingManager::CheckRenderLines(void) const
        {
            return m_lineCount > 0;
        }

        void DrawingManager::ConstructOverdrawPointMesh(unsigned& vertCount, unsigned& indexCount, ID3D11Buffer** mesh, ID3D11Buffer** indices) 
        {
            vertCount = m_pointCountOverdraw;

            // UPDATE VERTEX BUFFER /////////////////////////////////////////
            HRESULT result;
            D3D11_MAPPED_SUBRESOURCE mappedResource;

            //buffer of the data
            PrimitiveVertex *buffer;

            //lock the buffer
            result = m_deviceContext->Map(m_vertPointBufferOverdraw, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            //get the buffer
            buffer = (PrimitiveVertex*)mappedResource.pData;

            //copy the data over
            memcpy(buffer, &m_pointListOverdraw[ 0 ], sizeof(PrimitiveVertex) * vertCount);

            //unlock
            m_deviceContext->Unmap(m_vertPointBufferOverdraw, 0);

            indexCount = vertCount;
            *indices = m_indexBufferOverdraw;
            *mesh = m_vertPointBufferOverdraw;
        }

        void DrawingManager::ConstructOverdrawLineMesh(unsigned& vertCount, unsigned& indexCount, ID3D11Buffer** mesh, ID3D11Buffer** indices)
        {
            vertCount = m_lineCountOverdraw;

            // UPDATE VERTEX BUFFER /////////////////////////////////////////
            HRESULT result;
            D3D11_MAPPED_SUBRESOURCE mappedResource;

            //buffer of the data
            PrimitiveVertex *buffer;

            //lock the buffer
            result = m_deviceContext->Map(m_vertLineBufferOverdraw, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            //get the buffer
            buffer = (PrimitiveVertex*)mappedResource.pData;

            //copy the data over
            memcpy(buffer, &m_lineListOverdraw[ 0 ], sizeof(PrimitiveVertex) * vertCount);

            //unlock
            m_deviceContext->Unmap(m_vertLineBufferOverdraw, 0);

            indexCount = vertCount;
            *indices = m_indexBufferOverdraw;
            *mesh = m_vertLineBufferOverdraw;
        }

        bool DrawingManager::CheckOverdrawRenderPoints() const 
        {
            return m_pointCountOverdraw > 0;
        }

        bool DrawingManager::CheckOverdrawRenderLines() const 
        {
            return m_lineCountOverdraw > 0;
        }
    }
}
