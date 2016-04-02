#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class AudioItemEventReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            AudioItemEventReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "AudioItemEventReader" )
        );
    }
}