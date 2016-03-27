#include "UrsinePrecompiled.h"

#include "ArchetypeProcessor.h"
#include "ArchetypeData.h"

#include "JsonData.h"

namespace ursine
{
    rp::ArchetypeProcessor::ArchetypeProcessor(void) { }

    resources::ResourceData::Handle rp::ArchetypeProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        auto *json = expectInput<JsonData>( input );

        return std::make_shared<ArchetypeData>( json->GetData( ) );
    }
}