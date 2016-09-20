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
#include "VertexDefinitions.h"

namespace ursine
{
    namespace graphics
    {
        Mesh::Mesh()
            : m_name("UNDEFINED")
            , m_vertexBuffer(nullptr)
            , m_indexBuffer(nullptr)
            , m_material()
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

        std::vector<Vec3> &Mesh::GetRawNormals(void)
        {
            return m_rawNormData;
        }

        std::vector<Vec3> &Mesh::GetRawBinormals(void)
        {
            return m_rawNBinormData;
        }

        std::vector<Vec3> &Mesh::GetRawTangent(void)
        {
            return m_rawTangentmData;
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

        unsigned Mesh::GetNormalCount(void) const
        {
            return static_cast<unsigned>(m_rawNormData.size());
        }

        void Mesh::SetNormalCount(const unsigned normalCount)
        {
            m_rawNormData.resize(normalCount);
        }

        unsigned Mesh::GetBinormalCount(void) const
        {
            return static_cast<unsigned>(m_rawNBinormData.size());
        }

        void Mesh::SetBinormalCount(const unsigned normalCount)
        {
            m_rawNBinormData.resize(normalCount);
        }

        unsigned Mesh::GetTangentCount(void) const
        {
            return static_cast<unsigned>(m_rawTangentmData.size());
        }

        void Mesh::SetTangentCount(const unsigned normalCount)
        {
            m_rawTangentmData.resize(normalCount);
        }

        const SMat4 & Mesh::GetLocalToParentTransform(void) const
        {
            return m_localToParent;
        }

        void Mesh::SetLocalToParentTransform(const SMat4 &transform)
        {
            m_localToParent = transform;
        }

        std::vector<AnimationVertex> & Mesh::GetRawModelData()
        {
            return m_rawModelData;
        }
    }
}