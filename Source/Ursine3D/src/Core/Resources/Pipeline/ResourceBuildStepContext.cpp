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

        generatedResources.emplace( generated );

        return generated;
    }
}