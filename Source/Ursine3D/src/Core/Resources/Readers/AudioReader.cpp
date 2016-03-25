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