/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioItemEventReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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