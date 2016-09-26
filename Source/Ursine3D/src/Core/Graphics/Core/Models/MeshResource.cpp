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
        MeshResource::MeshResource()
            : m_name("UNDEFINED")
            , m_vertexBuffer(nullptr)
            , m_indexBuffer(nullptr)
        {
        }

        MeshResource::~MeshResource(void)
        {
            RELEASE_RESOURCE(m_vertexBuffer);
            RELEASE_RESOURCE(m_indexBuffer);;
        }

        const std::string &MeshResource::GetName(void) const
        {
            return m_name;
        }

        void MeshResource::SetName(const std::string & name)
        {
            m_name = name;
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
    }
}