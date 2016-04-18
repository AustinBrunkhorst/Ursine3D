/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureProcessorOptions.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class TextureProcessorOptions : public ResourceProcessorOptions
            {
                RESOURCE_PROCESSOR_OPTIONS;

            public:
                TextureProcessorOptions(void) = default;
            } Meta(Enable);
        }
    }
}