/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Model.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ModelResource.h"

namespace ursine
{
    namespace graphics
    {
        ModelResource::ModelResource(void)
            : m_rootNode( nullptr )
            , m_meshArray( )
            , m_meshMap( )
            , m_onGPU( false )
            , m_referenceCount( 0 )
        {
        }

        ModelResource::~ModelResource(void)
        {
            for (auto *x : m_meshArray)
                delete x;
        }

        void ModelResource::AddMesh(Mesh *newMesh)
        {
            if (m_meshArray.size() == 0)
                m_rootNode = newMesh;

            m_meshMap[newMesh->GetName()] = newMesh;
            m_meshArray.push_back(newMesh);
        }

        Mesh *ModelResource::GetMesh(const unsigned index) const
        {
            if(!(index < m_meshArray.size()))
            {
                return m_meshArray[ 0 ];
            }
            
            return m_meshArray[index];
        }

        Mesh *ModelResource::GetMesh(const std::string & name)
        {
            return m_meshMap[name];
        }

        unsigned ModelResource::GetMeshCount(void) const
        {
            return static_cast<unsigned>(m_meshArray.size());
        }

        const std::vector<Mesh*> &ModelResource::GetMeshArray(void) const
        {
            return m_meshArray;
        }
        
        void ModelResource::IncrementReference(void)
        {
            ++m_referenceCount;
        }

        void ModelResource::DecrementReference(void)
        {
            if(m_referenceCount == 0)
                return;
            --m_referenceCount;
        }

        void ModelResource::NoReference(void)
        {
            m_referenceCount = 0;
        }

        bool ModelResource::HasNoReferences(void) const
        {
            return m_referenceCount == 0;
        }

        bool ModelResource::GetIsLoaded() const
        {
            return m_onGPU;
        }

        void ModelResource::SetIsLoaded(bool isOnGPU)
        {
            m_onGPU = isOnGPU;
        }
    }
}