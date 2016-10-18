/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "MeshData.h"

namespace ursine
{
    namespace resources
    {
        class ModelData : public ResourceData
        {
            RESOURCE_DATA(ModelData);

            friend class ModelReader;

        public:
            ModelData(void);

            std::vector<MeshData::Handle> meshes;

            std::string name;

            graphics::GfxHND GetGraphicsHandle(void) const;

            void Write(pipeline::ResourceWriter &output) override;
            meta::Type GetReaderType(void) override;

        private:

            graphics::GfxHND m_id;

        } Meta(Register);
    }
}
