/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    namespace resources
    {
        // TODO: [J] This shouldn't be here?
        // I don't think we need a resource reader/writer
        class UMeshData : public ResourceData
        {
            RESOURCE_DATA;

        public:

            // TODO: [J] Encapsulate?
            std::vector<Vec3> verts;
            std::vector<uint> indices;
            std::vector<Vec3> normals;
            std::vector<Vec3> binormal;
            std::vector<Vec3> tangent;

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;

        } Meta(Register);
    }
}
