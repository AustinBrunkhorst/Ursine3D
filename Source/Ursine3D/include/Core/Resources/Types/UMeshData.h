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

namespace ursine
{
    namespace resources
    {
        // TODO: [J] Make this a resource of it's own
        class UMeshData //: public ResourceData
        {
            //RESOURCE_DATA;

            friend class UModelData;

        public:

            typedef std::shared_ptr<UMeshData> Handle;

            // TODO: [J] Encapsulate?
            std::vector<Vec3> verts;
            std::vector<uint> indices;
            std::vector<Vec3> normals;
            std::vector<Vec3> tangents;
            std::vector<Vec3> bitangents;

            void SetName(const std::string &name);
            const std::string &GetName(void) const;

        private:
            void Write(pipeline::ResourceWriter &output);// override;

            meta::Type GetReaderType(void);// override;

            std::string m_name;

        } Meta(Register);
    }
}
