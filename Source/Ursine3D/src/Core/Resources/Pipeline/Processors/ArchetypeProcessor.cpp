#include "UrsinePrecompiled.h"

#include "ArchetypeProcessor.h"
#include "ArchetypeData.h"

#include "JsonData.h"

namespace ursine
{
    rp::ArchetypeProcessor::ArchetypeProcessor(void) { }

    resources::ResourceData::Handle rp::ArchetypeProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        UAssertCatchable( input && input->GetType( ) == typeof( JsonData ),
            "Invalid input to archetype processor."
        );

        auto *jsonData = static_cast<JsonData*>( input.get( ) );

        return std::make_shared<ArchetypeData>( jsonData->GetData( ) );
    }
}