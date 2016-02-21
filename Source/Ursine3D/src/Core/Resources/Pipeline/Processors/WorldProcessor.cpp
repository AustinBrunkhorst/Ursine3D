#include "UrsinePrecompiled.h"

#include "WorldProcessor.h"

namespace ursine
{
    rp::WorldProcessor::WorldProcessor(void) { }

    resources::ResourceData::Handle rp::WorldProcessor::Process(resources::ResourceData::Handle input, const ResourceProcessorContext &context)
    {
        return input;
    }
}