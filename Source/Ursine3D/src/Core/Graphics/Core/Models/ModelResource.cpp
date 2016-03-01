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
            : m_rootNode(nullptr)
            , m_meshArray()
            , m_meshMap()
            , m_meshHierarchy()
            , m_rigHierarchy()
            , m_onGPU(false)
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

        Mesh *ModelResource::GetMesh(const unsigned index)
        {
            UAssert(index < m_meshArray.size(), "Tried to get mesh that does not exist!");
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

        const std::vector<ufmt_loader::MeshInLvl> &ModelResource::GetMeshLvlArray(void) const
        {
            return m_meshHierarchy;
        }

        const std::vector<ufmt_loader::RigInLvl> &ModelResource::GetRigLvlArray(void) const
        {
            return m_rigHierarchy;
        }

        void ModelResource::AddMesh2Tree(const ufmt_loader::MeshInLvl& meshLvl)
        {
            m_meshHierarchy.push_back(meshLvl);
        }

        void ModelResource::AddRig2Tree(const ufmt_loader::RigInLvl& rigLvl)
        {
            m_rigHierarchy.push_back(rigLvl);
        }

        void ModelResource::IncrementReference(void)
        {
            ++m_referenceCount;
        }

        void ModelResource::DecrementReference(void)
        {
            UAssert(m_referenceCount != 0, "Attempted to unload model that was already unloaded!");
            --m_referenceCount;
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