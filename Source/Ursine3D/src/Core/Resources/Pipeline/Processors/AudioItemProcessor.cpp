#include "UrsinePrecompiled.h"

#include "AudioItemProcessor.h"

#include "JsonData.h"
#include "AudioItemEventData.h"

namespace ursine
{
    rp::AudioItemProcessor::AudioItemProcessor(void) { }

    resources::ResourceData::Handle rp::AudioItemProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        auto *json = expectInput<JsonData>( input );

        URSINE_TODO( "Return type based on type of audio item" );
        return std::make_shared<AudioItemEventData>( json->GetData( ) );
    }
}