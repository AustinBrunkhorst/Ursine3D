#include "UrsinePrecompiled.h"

#include "JsonReader.h"
#include "JsonData.h"

namespace ursine
{
    namespace resources
    {
        JsonReader::JsonReader(void) { }

        ResourceData::Handle JsonReader::Read(ResourceReader &input)
        {
            std::string jsonString;

            input.Read( jsonString );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            UAssertCatchable( jsonError.empty( ),
                "Error reading json.\nerror: %s",
                jsonError.c_str( )
            );

            return std::make_shared<JsonData>( json );
        }
    }
}