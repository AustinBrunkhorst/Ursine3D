/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceProcessorContext.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceBuildStepContext.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager;

            struct ResourceProcessorContext : ResourceBuildStepContext
            {
                ResourceProcessorContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource)
                    : ResourceBuildStepContext( pipeline, resource ) { }
            };
        }
    }
}