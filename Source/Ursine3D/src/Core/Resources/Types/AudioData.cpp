#include "UrsinePrecompiled.h"

#include "AudioData.h"
#include "AudioReader.h"

namespace ursine
{
    namespace resources
    {
        AudioData::AudioData(void) { }

        AudioData::~AudioData(void) { }

        void AudioData::Write(pipeline::ResourceWriter &output)
        {

        }

        meta::Type AudioData::GetReaderType(void)
        {
            return typeof( AudioReader );
        }
    }
}