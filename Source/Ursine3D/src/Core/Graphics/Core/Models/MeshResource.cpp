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

        MeshResource::MeshResource(void)
            : m_name( "UNDEFINED" )
            , m_vertexBuffer( nullptr )
            , m_indexBuffer( nullptr )
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

        const SMat4 & MeshResource::GetLocalToParentTransform(void) const
        {
            return m_localToParent;
        }

        void MeshResource::SetLocalToParentTransform(const SMat4 &transform)
        {
            m_localToParent = transform;
        }

        uint MeshResource::GetVertexCount(void) const
        {
            return static_cast<uint>( m_meshData->verts.size( ) );
        }

        Vec3 *MeshResource::GetVertexData(void) const
        {
            return &m_meshData->verts[ 0 ];
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
    }
}