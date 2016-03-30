#include "UrsinePrecompiled.h"

#include "UIScreenProcessor.h"

#include "JsonData.h"
#include "UIScreenData.h"

namespace ursine
{
    rp::UIScreenProcessor::UIScreenProcessor(void) { }

    resources::ResourceData::Handle rp::UIScreenProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        auto *json = expectInput<JsonData>( input );

        return std::make_shared<UIScreenData>( json->GetData( ) );
    }
}