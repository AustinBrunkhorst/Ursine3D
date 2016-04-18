/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ArchetypeReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ArchetypeReader.h"
#include "ArchetypeData.h"

namespace ursine
{
    namespace resources
    {
        ArchetypeReader::ArchetypeReader(void) { }

        ResourceData::Handle ArchetypeReader::Read(ResourceReader &input)
        {
            std::string jsonString;

            input.Read( jsonString );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            UAssertCatchable( jsonError.empty( ),
                "Error parsing archetype JSON.\nerror: %s",
                jsonError.c_str( )
            );

            return std::make_shared<ArchetypeData>( json );
        }
    }
}