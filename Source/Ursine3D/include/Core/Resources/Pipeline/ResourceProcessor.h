#pragma once

#include "ResourceProcessorContext.h"
#include "ResourcePipelineProperties.h"
#include "ResourceData.h"

#define RESOURCE_PROCESSOR META_OBJECT

namespace ursine
{
    namespace resources
    {
        namespace pipeline 
        {
            class ResourceProcessor : public meta::Object
            {
            public:
                typedef std::shared_ptr<ResourceProcessor> Handle;

            private:
                friend class ResourcePipelineManager;

                virtual ResourceData::Handle Process(ResourceData::Handle input, const ResourceProcessorContext &context) = 0;
            };
        }
    }
}