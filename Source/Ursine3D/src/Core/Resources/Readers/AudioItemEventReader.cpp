#include "UrsinePrecompiled.h"

#include "AudioItemEventReader.h"
#include "AudioItemEventData.h"

namespace ursine
{
    namespace resources
    {
        AudioItemEventReader::AudioItemEventReader(void) { }

        ResourceData::Handle AudioItemEventReader::Read(ResourceReader &input)
        {
            std::string jsonString;

            input.Read( jsonString );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            UAssertCatchable( jsonError.empty( ),
                "Error parsing audio event JSON.\nerror: %s",
                jsonError.c_str( )
            );

            return std::make_shared<AudioItemEventData>( json );
        }
    }
}