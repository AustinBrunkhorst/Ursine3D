/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIScreenReader.h
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
        class UIScreenReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            UIScreenReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "UIScreenReader" )
        );
    }
}