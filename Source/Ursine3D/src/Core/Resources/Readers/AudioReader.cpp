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
                input.ReadString( e );

            void *initBytes;
            size_t initSize;

            input.Read( initSize );

            initBytes = new uint8[ initSize ];

            input.ReadBytes( initBytes, initSize );

            void *bankBytes;
            size_t bankSize;

            input.Read( bankSize );

            bankBytes = new uint8[ bankSize ];

            input.ReadBytes( bankBytes, bankSize );

            auto resource = std::make_shared<AudioData>( 
                events, 
                initBytes, 
                initSize, 
                bankBytes, 
                bankSize
            );

            delete[] initBytes;
            delete[] bankBytes;

            return resource;
        }
    }
}