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

            protected:
                template<typename InputType>
                InputType *expectInput(ResourceData::Handle input);

            private:
                friend class ResourcePipelineManager;

                virtual ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) = 0;

                virtual bool BuildPreview(ResourceProcessorContext &context, const Vec2 &maxDimensions)
                {
                    return false;
                }
            };
        }
    }
}

#include "ResourceProcessor.hpp"