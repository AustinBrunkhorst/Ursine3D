/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceBuildStepContext.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ResourceBuildStepContext.h"

#include "ResourcePipelineManager.h"

namespace ursine
{
    rp::ResourceBuildStepContext::ResourceBuildStepContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource)
        : pipeline( pipeline )
        , resource( resource ) { }

    rp::ResourceItem::Handle rp::ResourceBuildStepContext::AllocateGeneratedResource(const fs::path &sourceFile)
    {
        auto generated = pipeline->allocateGeneratedResource( resource, sourceFile );

        generatedResources.emplace_back( generated );

        return generated;
    }
}