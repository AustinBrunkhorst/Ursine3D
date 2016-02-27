#pragma once

#include "ResourceProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class WorldProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                WorldProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "WorldProcessor" )
            );
        }
    }
}