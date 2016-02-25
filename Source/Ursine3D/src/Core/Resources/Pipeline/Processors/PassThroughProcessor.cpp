#include "UrsinePrecompiled.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    rp::PassThroughProcessor::PassThroughProcessor(void) { }

    resources::ResourceData::Handle rp::PassThroughProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        return input;
    }
}