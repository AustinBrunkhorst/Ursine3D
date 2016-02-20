#include "UrsinePrecompiled.h"

#include "TextureProcessor.h"

namespace ursine
{
    rp::TextureProcessor::TextureProcessor(void) { }

    resources::ResourceData::Handle rp::TextureProcessor::Process(resources::ResourceData::Handle input, const ResourceProcessorContext &context)
    {
        return input;
    }
}