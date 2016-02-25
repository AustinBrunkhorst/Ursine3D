#include "UrsinePrecompiled.h"

#include "WorldProcessor.h"
#include "WorldData.h"

#include "JsonData.h"

namespace ursine
{
    rp::WorldProcessor::WorldProcessor(void) { }

    resources::ResourceData::Handle rp::WorldProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        UAssert( input && input->GetType( ) == typeof( JsonData ),
            "Invalid input to world processor."
        );

        auto *jsonData = static_cast<JsonData*>( input.get( ) );

        return std::make_shared<WorldData>( jsonData->GetData( ) );
    }
}