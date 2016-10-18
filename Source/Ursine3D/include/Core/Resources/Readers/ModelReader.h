/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelReader.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceTypeReader.h"
#include "ModelData.h"

namespace ursine
{
    namespace resources
    {
        class ModelReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            ModelReader(void);

            ursine::resources::ResourceData::Handle Read(ResourceReader &input) override;
            ursine::resources::ModelData::Handle ReadJDL(ResourceReader &input);

        } Meta(
            Enable,
            DisplayName( "ModelReader" )
        );
    }
}
