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
#include "MeshResource.h"

namespace ursine
{
    namespace graphics
    {
        // TODO: [J] Keep this?
        class ModelResource
        {
        public:
            ModelResource(void);
            ~ModelResource(void);

            // add a mesh to this model
            void AddMesh(MeshResource *newMesh);

            // get mesh by index or name
            MeshResource *GetMesh(const unsigned index) const;
            MeshResource *GetMesh(const std::string &name);

            // get num of meshes
            unsigned GetMeshCount(void) const;

            // get the vector of meshes
            const std::vector<MeshResource *> &GetMeshArray(void) const;
            
            void IncrementReference(void);
            void DecrementReference(void);
            void NoReference(void);

            bool HasNoReferences(void) const;

            bool GetIsLoaded(void) const;
            void SetIsLoaded(bool isOnGPU);

        private:
            // root mesh of this model
            MeshResource *m_rootNode;

            // array of all meshes in this array
            std::vector<MeshResource *> m_meshArray;

            // map for name lookup... This might need to get changed
            std::unordered_map<std::string, MeshResource *> m_meshMap;
            
            bool m_onGPU;

            unsigned m_referenceCount;
        };
    }
}