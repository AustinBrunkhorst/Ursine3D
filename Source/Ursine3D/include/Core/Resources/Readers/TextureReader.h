/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureReader.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
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
        class TextureReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            TextureReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "TextureReader" )
        );
    }
}