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

            input.ReadString( jsonString );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            return std::make_shared<JsonData>( json );
        }
    }
}