/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelData.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "UMeshData.h"

namespace ursine
{
    namespace resources
    {
        class UModelData : public ResourceData
        {
            RESOURCE_DATA(UModelData);

            friend class UModelReader;

        public:

            std::vector<UMeshData::Handle> meshes;

            std::string name;

            graphics::GfxHND GetGraphicsHandle(void) const;

            void Write(pipeline::ResourceWriter &output) override;
            meta::Type GetReaderType(void) override;

        private:

            graphics::GfxHND m_id;

        } Meta(Register);
    }
}
