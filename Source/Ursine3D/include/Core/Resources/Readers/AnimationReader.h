/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationReader.h
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
        class AnimationReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            AnimationReader(void);

            ursine::resources::ResourceData::Handle Read(ResourceReader &input) override;

        } Meta(
            Enable,
            DisplayName( "AnimationReader" )
        );
    }
}
