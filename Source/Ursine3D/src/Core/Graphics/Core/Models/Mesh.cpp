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
#include "Mesh.h"
#include "..\..\..\..\..\include\Core\Graphics\Core\Models\Mesh.h"

namespace ursine
{
    namespace graphics
    {
        Mesh::Mesh()
            : m_name("UNDEFINED")
            , m_vertexBuffer(nullptr)
            , m_indexBuffer(nullptr)
            , m_material()
            , m_parent(nullptr)
        {
        }

        Mesh::~Mesh(void)
        {
            RELEASE_RESOURCE(m_vertexBuffer);
            RELEASE_RESOURCE(m_indexBuffer);;
        }

        const std::string &Mesh::GetName(void) const
        {
            return m_name;
        }

        void Mesh::SetName(const std::string & name)
        {
            m_name = name;
        }

        unsigned Mesh::GetID(void) const
        {
            return m_id;
        }

        void Mesh::SetID(const unsigned id)
        {
            m_id = id;
        }

        std::vector<Vec3>& Mesh::GetRawVertices(void)
        {
            return m_rawVertData;
        }

        std::vector<UINT>& Mesh::GetRawIndices(void)
        {
            return m_rawIndexData;
        }

        ID3D11Buffer *&Mesh::GetVertexBuffer(void)
        {
            return m_vertexBuffer;
        }

        unsigned Mesh::GetVertexCount(void) const
        {
            return static_cast<unsigned>(m_rawVertData.size());
        }

        void Mesh::SetVertexCount(const unsigned vertCount)
        {
            m_rawVertData.resize(vertCount);
        }

        ID3D11Buffer *&Mesh::GetIndexBuffer(void)
        {
            return m_indexBuffer;
        }

        unsigned Mesh::GetIndexCount(void) const
        {
            return static_cast<unsigned>(m_rawIndexData.size( ));
        }

        void Mesh::SetIndexCount(const unsigned indexCount)
        {
            m_rawIndexData.resize(indexCount);
        }

        const Material &Mesh::GetMaterial(void)
        {
            return m_material;
        }
        Mesh *Mesh::GetParent(void)
        {
            return m_parent;
        }
        void Mesh::SetParent(Mesh *parent)
        {
            m_parent = parent;
        }
        const SMat4 & Mesh::GetLocalToParentTransform(void) const
        {
            return m_localToParent;
        }
        void Mesh::SetLocalToParentTransform(const SMat4 &transform)
        {
            m_localToParent = transform;
        }
    }
}