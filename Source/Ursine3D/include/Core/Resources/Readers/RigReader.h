/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** RigReader.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class RigReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            RigReader(void);

            ursine::resources::ResourceData::Handle Read(ResourceReader &input) override;

        } Meta(
            Enable,
            DisplayName( "RigReader" )
        );
    }
}
