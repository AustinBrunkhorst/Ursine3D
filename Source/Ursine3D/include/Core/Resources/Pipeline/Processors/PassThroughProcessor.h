#include "ResourceProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            // Processor that simply returns the input
            class PassThroughProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                PassThroughProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, const ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "PassThroughProcessor" )
            );
        }
    }
}