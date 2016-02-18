#include "UrsinePrecompiled.h"

#include "TextureProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            TextureProcessor::TextureProcessor(void) { }

            ResourceData::Handle TextureProcessor::Process(ResourceData::Handle input, const ResourceProcessorContext &context)
            {
                return input;
            }
        }
    }
}