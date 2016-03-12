#include "UrsinePrecompiled.h"

#include "UIScreenProcessor.h"

namespace ursine
{
    rp::UIScreenProcessor::UIScreenProcessor(void) { }

    resources::ResourceData::Handle rp::UIScreenProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        return input;
    }
}