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
    namespace resources
    {
        class UModelReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            UModelReader(void);

            ursine::resources::ResourceData::Handle Read(ResourceReader &input) override;
            ursine::resources::UModelData::Handle ReadJDL(ResourceReader &input);

        } Meta(
            Enable,
            DisplayName( "UModelReader" )
        );
    }
}
