#include "UrsinePrecompiled.h"

#include "WorldReader.h"
#include "WorldData.h"

namespace ursine
{
    namespace resources
    {
        WorldReader::WorldReader(void) { }

        ResourceData::Handle WorldReader::Read(ResourceReader &input)
        {
            uint64 jsonLength;
            std::string jsonString;

            input >> jsonLength;

            jsonString.resize( static_cast<size_t>( jsonLength ) );

            input.ReadBytes( &jsonString[ 0 ], jsonLength );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            return std::make_shared<WorldData>( json );
        }
    }
}