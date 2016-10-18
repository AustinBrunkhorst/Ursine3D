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
#include "ModelData.h"

namespace ursine
{
    namespace graphics
    {
        // TODO: [J] Keep this?
        class ModelResource
        {
            friend class ModelManager;

        public:
            ModelResource(const resources::ModelData::Handle &modelData);
            ~ModelResource(void);

            MeshResource *GetMesh(uint index) const;
            MeshResource *GetMeshByName(const std::string &name);

            unsigned GetMeshCount(void) const;

            const std::vector<MeshResource *> &GetMeshArray(void) const;

            const std::string &GetName(void) const;

            void IncrementReference(void);
            void DecrementReference(void);
            void NoReference(void);

            bool HasNoReferences(void) const;

            bool GetIsLoaded(void) const;
            void SetIsLoaded(bool isOnGPU);

        private:
            void addMesh(MeshResource *mesh);

            // array of all meshes in this array
            std::vector<MeshResource *> m_meshArray;

            // map for name lookup... This might need to get changed
            std::unordered_map<std::string, MeshResource *> m_meshMap;
            
            bool m_onGPU;

            unsigned m_referenceCount;

            resources::ModelData::Handle m_modelData;
        };
    }
}