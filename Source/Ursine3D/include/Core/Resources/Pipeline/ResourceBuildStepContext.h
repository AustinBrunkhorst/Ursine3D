/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceBuildStepContext.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceItem.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager;

            struct ResourceBuildStepContext
            {
                ResourcePipelineManager *pipeline;
                ResourceItem::Handle resource;

                ResourceItem::List generatedResources;

                ResourceBuildStepContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource);

                ResourceItem::Handle AllocateGeneratedResource(const fs::path &sourceFile);
            };
        }
    }
}