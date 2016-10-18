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
        MeshResource::MeshResource(const resources::MeshData::Handle &mesh)
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

        std::vector<Vec3> MeshResource::GetVertexArray(void) const
        {
            std::vector<Vec3> vertexPositions;

            for (auto &vert : m_meshData->verts)
            {
                vertexPositions.push_back( vert.position );
            }

            return vertexPositions;
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

            for (size_t i = 0; i < size; ++i)
            {
                auto &vert = buffer[ i ];
                auto &meshVert = verts[ i ];

                vert.vPos = DirectX::XMFLOAT3( meshVert.position.GetFloatPtr( ) );
                vert.vNor = DirectX::XMFLOAT3( meshVert.normal.GetFloatPtr( ) );
                vert.vTan = DirectX::XMFLOAT3( meshVert.tangent.GetFloatPtr( ) );

                vert.vUv = DirectX::XMFLOAT2( meshVert.uv.GetFloatPtr( ) );

                vert.vBWeight = DirectX::XMFLOAT4( meshVert.boneWeights.GetFloatPtr( ) );
                vert.vBIdx[ 0 ] = static_cast<BYTE>( meshVert.boneIndices[ 0 ] );
                vert.vBIdx[ 1 ] = static_cast<BYTE>( meshVert.boneIndices[ 1 ] );
                vert.vBIdx[ 2 ] = static_cast<BYTE>( meshVert.boneIndices[ 2 ] );
                vert.vBIdx[ 3 ] = static_cast<BYTE>( meshVert.boneIndices[ 3 ] );
            }
        }
    }
}