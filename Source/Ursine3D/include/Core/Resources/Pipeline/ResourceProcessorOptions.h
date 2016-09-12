/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceProcessorOptions.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Meta.h"

#define RESOURCE_PROCESSOR_OPTIONS META_OBJECT

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceProcessorOptions : public meta::Object
            {
            public:
                typedef std::shared_ptr<ResourceProcessorOptions> Handle;
            };
        }
    }
}