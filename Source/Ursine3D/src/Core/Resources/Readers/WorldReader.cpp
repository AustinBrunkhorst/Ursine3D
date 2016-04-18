/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
            std::string jsonString;

            input.Read( jsonString );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            UAssertCatchable( jsonError.empty( ),
                "Error parsing world JSON.\nerror: %s",
                jsonError.c_str( )
            );

            return std::make_shared<WorldData>( json );
        }
    }
}