/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelReader.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceTypeReader.h"
#include "UModelData.h"

namespace ursine
{
    // TODO: [J] Remove this
    namespace graphics
    {
        class ModelManager;
    }

    namespace resources
    {
        namespace pipeline
        {
            // TODO: [J] Remove this shit
            class UModelImporter;
        }

        class UModelReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

            // TODO: [J] Remove this shit
            friend class pipeline::UModelImporter;
            friend class graphics::ModelManager;

        public:
            UModelReader(void);

            UModelData::Handle ReadJDL(ResourceReader &input);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "UModelReader" )
        );
    }
}
