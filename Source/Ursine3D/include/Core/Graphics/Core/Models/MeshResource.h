/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Mesh.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <string>

#include "D3D11Forward.h"
#include "UMeshData.h"
#include "SMat4.h"
#include "VertexDefinitions.h"

namespace ursine
{
    namespace graphics
    {
        // TODO: [J] Find a way to get rid of this?
        class MeshResource
        {
            friend class ModelManager;

        public:
            // constructor
            MeshResource(const resources::UMeshData::Handle &meshData);

            ~MeshResource(void);

            // set name stuff
            const std::string &GetName(void) const;

            unsigned GetID(void) const;
            void SetID(const unsigned id);

            ID3D11Buffer *&GetVertexBuffer(void);
            ID3D11Buffer *&GetIndexBuffer(void);

            uint GetVertexCount(void) const;
            std::vector<Vec3> GetVertexArray(void) const;

            uint GetIndexCount(void) const;
            uint *GetIndexData(void) const;
            const std::vector<uint> &GetIndexArray(void) const;

            void CreateVertexBufferData(std::vector<AnimationVertex> &buffer) const;

        private:
            // name
            std::string m_name;

            // id of this mesh, which is the index in the model's array
            unsigned m_id;

            // raw dx11 data
            ID3D11Buffer *m_vertexBuffer;
            ID3D11Buffer *m_indexBuffer;

            resources::UMeshData::Handle m_meshData;
        };
    }
}