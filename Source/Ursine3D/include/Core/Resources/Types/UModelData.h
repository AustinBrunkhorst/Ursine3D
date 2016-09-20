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

namespace ursine
{
    namespace resources
    {
        class UModelData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            UModelData(const UModelData &other);
            ~UModelData(void);

            void AddMesh(graphics::Mesh *mesh);
            graphics::Mesh *GetMesh(uint index) const;

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;

            // TODO: [J] Look into not dynamically allocating these
            std::vector<graphics::Mesh*> m_meshes;

            // TODO: [J] Functions for this
            bool m_onGPU;

        } Meta(Register);
    }
}
