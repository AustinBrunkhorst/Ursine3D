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
#include "Material.h"
#include "Vec3.h"
#include "SMat4.h"

namespace ursine
{
    namespace graphics
    {
        struct AnimationVertex;

        // TODO: [J] Find a way to get rid of this?
        class MeshResource
        {
        public:
            // constructor
            MeshResource();

            ~MeshResource(void);

            // set name stuff
            const std::string &GetName(void) const;
            void SetName(const std::string &name);

            unsigned GetID(void) const;
            void SetID(const unsigned id);

            ID3D11Buffer *&GetVertexBuffer(void);
            ID3D11Buffer *&GetIndexBuffer(void);

            const SMat4 &GetLocalToParentTransform(void) const;
            void SetLocalToParentTransform(const SMat4 &transform);

        private:
            // name
            std::string m_name;

            // id of this mesh, which is the index in the model's array
            unsigned m_id;

            // raw dx11 data
            ID3D11Buffer *m_vertexBuffer;
            ID3D11Buffer *m_indexBuffer;

            // transform to place this mesh in parent's space
            SMat4 m_localToParent;
        };
    }
}