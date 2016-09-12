/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioReader.h"
#include "AudioData.h"

namespace ursine
{
    namespace resources
    {
        AudioReader::AudioReader(void) { }

        ResourceData::Handle AudioReader::Read(ResourceReader &input)
        {
            unsigned eventCount;

            input.Read( eventCount );

            AudioData::EventList events( eventCount );

            for (auto &e : events)
                input.Read( e );

            BinaryData initData;

            input.Read( initData );

            BinaryData bankData;

            input.Read( bankData );

            auto resource = std::make_shared<AudioData>( 
                events, 
                std::move( initData ),
                std::move( bankData )
            );

            return resource;
        }
    }
}