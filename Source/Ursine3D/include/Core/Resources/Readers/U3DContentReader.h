/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** U3DContentReader.h
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
        class U3DContentReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            U3DContentReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "U3DContentReader" )
        );
    }
}
