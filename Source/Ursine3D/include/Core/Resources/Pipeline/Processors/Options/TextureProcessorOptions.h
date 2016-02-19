#include "ResourceProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class TextureProcessorOptions : public ResourceProcessorOptions
            {
                RESOURCE_PROCESSOR_OPTIONS;

            public:
                TextureProcessorOptions(void) = default;
            } Meta(Enable);
        }
    }
}