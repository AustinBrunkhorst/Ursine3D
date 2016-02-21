#include "ResourceProcessor.h"

#include "Options/TextureProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class TextureProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                TextureProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, const ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "TextureProcessor" ),
                ResourceProcessorConfig( 
                    typeof( ursine::rp::TextureProcessorOptions )
                )
            );
        }
    }
}