#pragma once

#include "ResourceProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ArchetypeProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                ArchetypeProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "ArchetypeProcessor" )
            );
        }
    }
}