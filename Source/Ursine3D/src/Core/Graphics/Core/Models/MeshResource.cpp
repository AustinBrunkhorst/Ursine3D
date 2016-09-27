/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Mesh.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include <d3d11.h>

#include "GfxDefines.h"
#include "MeshResource.h"
#include "VertexDefinitions.h"

namespace ursine
{
    namespace graphics
    {
        MeshResource::MeshResource(const resources::UMeshData::Handle &mesh)
            : m_name( mesh->GetName( ) )
            , m_vertexBuffer( nullptr )
            , m_indexBuffer( nullptr )
            , m_meshData( mesh )
        {
        }

        MeshResource::~MeshResource(void)
        {
            RELEASE_RESOURCE( m_vertexBuffer );
            RELEASE_RESOURCE( m_indexBuffer );
        }

        const std::string &MeshResource::GetName(void) const
        {
            return m_name;
        }

        unsigned MeshResource::GetID(void) const
        {
            return m_id;
        }

        void MeshResource::SetID(const unsigned id)
        {
            m_id = id;
        }

        ID3D11Buffer *&MeshResource::GetVertexBuffer(void)
        {
            return m_vertexBuffer;
        }

        ID3D11Buffer *&MeshResource::GetIndexBuffer(void)
        {
            return m_indexBuffer;
        }

        uint MeshResource::GetVertexCount(void) const
        {
            return static_cast<uint>( m_meshData->verts.size( ) );
        }

        const std::vector<Vec3> &MeshResource::GetVertexArray(void) const
        {
            return m_meshData->verts;
        }

        uint MeshResource::GetIndexCount(void) const
        {
            return static_cast<uint>( m_meshData->indices.size( ) );
        }

        uint *MeshResource::GetIndexData(void) const
        {
            return &m_meshData->indices[ 0 ];
        }

        const std::vector<uint> &MeshResource::GetIndexArray(void) const
        {
            return m_meshData->indices;
        }

        void MeshResource::CreateVertexBufferData(std::vector<AnimationVertex> &buffer) const
        {
            size_t size = m_meshData->verts.size( );

            buffer.resize( size );

            auto &verts = m_meshData->verts;
            auto &normals = m_meshData->normals;
            auto &tangents = m_meshData->tangents;

            for (size_t i = 0; i < size; ++i)
            {
                auto &vert = buffer[ i ];

                vert.vPos = DirectX::XMFLOAT3( verts[ i ].GetFloatPtr( ) );
                vert.vNor = DirectX::XMFLOAT3( normals[ i ].GetFloatPtr( ) );
                vert.vTan = DirectX::XMFLOAT3( tangents[ i ].GetFloatPtr( ) );

                // TODO: [J] UV data and bone weight data
                vert.vUv = DirectX::XMFLOAT2( 0.0f, 0.0f );

                vert.vBWeight = DirectX::XMFLOAT4( 0, 0, 0, 1 );
                vert.vBIdx[ 0 ] = static_cast<BYTE>( 0 );
                vert.vBIdx[ 1 ] = static_cast<BYTE>( 0 );
                vert.vBIdx[ 2 ] = static_cast<BYTE>( 0 );
                vert.vBIdx[ 3 ] = static_cast<BYTE>( 0 );
            }
        }
    }
}