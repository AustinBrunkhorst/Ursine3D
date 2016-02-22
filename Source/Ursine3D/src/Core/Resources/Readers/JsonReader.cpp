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
            uint64 jsonLength;
            std::string jsonString;

            input >> jsonLength;

            jsonString.resize( static_cast<size_t>( jsonLength ) );

            input.ReadBytes( &jsonString[ 0 ], jsonLength );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            return std::make_shared<JsonData>( json );
        }
    }
}