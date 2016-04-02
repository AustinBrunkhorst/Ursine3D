#pragma once

#include "ResourceProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class AudioItemProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                AudioItemProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "AudioItemProcessor" )
            );
        }
    }
}