/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UMeshData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "Vec3.h"
#include "Vec2.h"
#include "SVec4.h"

namespace ursine
{
    namespace resources
    {
        // TODO: [J] Make this a resource of it's own
        class UMeshData //: public ResourceData
        {
            //RESOURCE_DATA;

        public:

            // TODO: Remove this
            typedef std::shared_ptr<UMeshData> Handle;

            class Vertex
            {
            public:
                Vertex(void);

                Vec3 position;
                Vec3 normal;
                Vec3 tangent;
                Vec3 bitangent;
                // TODO: Support multiple uv coordinates
                Vec2 uv;
                uint boneIndices[4];
                SVec4 boneWeights;
            };

            std::vector<Vertex> verts;
            std::vector<uint> indices;

            void SetName(const std::string &name);
            const std::string &GetName(void) const;

            void Write(pipeline::ResourceWriter &output);// override;
            meta::Type GetReaderType(void);// override;

        private:

            std::string m_name;

        } Meta(Register);
    }
}
