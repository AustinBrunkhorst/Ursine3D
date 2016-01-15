/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
        class Mesh
        {
        public:
            // constructor
            Mesh();

            ~Mesh(void);

            // set name stuff
            const std::string &GetName(void) const;
            void SetName(const std::string &name);

            unsigned GetID(void) const;
            void SetID(const unsigned id);

            // raw data
            std::vector<Vec3> &GetRawVertices(void);
            std::vector<UINT> &GetRawIndices(void);
            
            // vert data
            ID3D11Buffer *&GetVertexBuffer(void);
            unsigned GetVertexCount(void) const;
            void SetVertexCount(const unsigned vertCount);

            // index data
            ID3D11Buffer *&GetIndexBuffer(void);
            unsigned GetIndexCount(void) const;
            void SetIndexCount(const unsigned indexCount);

            // getting material data
            const Material &GetMaterial(void);

            // parent data
            Mesh *GetParent(void);
            void SetParent(Mesh *parent);

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

            // raw mesh data for physics stuff
            std::vector<Vec3> m_rawVertData;
            std::vector<UINT> m_rawIndexData;

            // material data
            Material m_material;

            // what mesh is the parent of this mesh
            Mesh *m_parent;

            // transform to place this mesh in parent's space
            SMat4 m_localToParent;
        };
    }
}