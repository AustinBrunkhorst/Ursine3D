#include "UrsinePrecompiled.h"

#include "ResourceBuildStepContext.h"

namespace ursine
{
    void rp::ResourceBuildStepContext::AddGeneratedResource(const fs::path &resource)
    {
        generatedResources.emplace_back( resource );
    }
}