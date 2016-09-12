/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** JsonReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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