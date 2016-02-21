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
            // TODO:
            return std::make_shared<JsonData>( );
        }
    }
}