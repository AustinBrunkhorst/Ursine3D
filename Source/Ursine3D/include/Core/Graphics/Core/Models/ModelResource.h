/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelResource.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"
#include "Mesh.h"
#include "LvlHierarchy.h"

namespace ursine
{
    namespace graphics
    {
        class ModelResource
        {
        public:
            ModelResource(void);
            ~ModelResource(void);

            // add a mesh to this model
            void AddMesh(Mesh *newMesh);

            // get mesh by index or name
            Mesh *GetMesh(const unsigned index);
            Mesh *GetMesh(const std::string &name);

            // get num of meshes
            unsigned GetMeshCount(void) const;

            // get the vector of meshes
            const std::vector<Mesh *> &GetMeshArray(void) const;

            // get the vector of meshes
            const std::vector<ufmt_loader::MeshInLvl> &GetMeshLvlArray(void) const;
            // get the vector of meshes
            const std::vector<ufmt_loader::RigInLvl> &GetRigLvlArray(void) const;

            // add mesh name for hierarchy tree
            void AddMesh2Tree(const ufmt_loader::MeshInLvl& meshLvl);
            // add rig name for hierarchy tree
            void AddRig2Tree(const ufmt_loader::RigInLvl& rigLvl);

            void IncrementReference(void);
            void DecrementReference(void);

            bool HasNoReferences(void) const;

            bool GetIsLoaded(void) const;
            void SetIsLoaded(bool isOnGPU);

        private:
            // root mesh of this model
            Mesh *m_rootNode;

            // array of all meshes in this array
            std::vector<Mesh *> m_meshArray;

            // map for name lookup... This might need to get changed
            std::unordered_map<std::string, Mesh *> m_meshMap;

            // array of all mesh hierarchy
            std::vector<ufmt_loader::MeshInLvl> m_meshHierarchy;

            // aray of all rig hierarchy
            std::vector<ufmt_loader::RigInLvl> m_rigHierarchy;

            bool m_onGPU;

            unsigned m_referenceCount;
        };
    }
}