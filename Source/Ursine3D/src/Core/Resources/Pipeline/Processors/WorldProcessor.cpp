#include "UrsinePrecompiled.h"

#include "WorldProcessor.h"
#include "WorldData.h"

#include "JsonData.h"

namespace ursine
{
    rp::WorldProcessor::WorldProcessor(void) { }

    resources::ResourceData::Handle rp::WorldProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        auto *json = expectInput<JsonData>( input );

        return std::make_shared<WorldData>( json->GetData( ) );
    }
}