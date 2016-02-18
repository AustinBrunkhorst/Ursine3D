#include "ResourceProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            enum class TextureCompressionType
            {
                Compressed,
                ReallyCompressed,
                ReallyReallyCompressed
            } Meta(Enable);

            class TextureProcessorOptions : public ResourceProcessorOptions
            {
                RESOURCE_PROCESSOR_OPTIONS;

            public:
                TextureProcessorOptions(void) = default;

                bool generateMips { true };
                TextureCompressionType compression { TextureCompressionType::ReallyReallyCompressed };

            private:
            } Meta(Enable);
        }
    }
}