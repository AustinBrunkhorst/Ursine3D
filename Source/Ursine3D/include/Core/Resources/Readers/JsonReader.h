#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class JsonReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            JsonReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "JsonReader" )
        );
    }
}