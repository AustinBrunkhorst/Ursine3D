#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class FontReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            FontReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "FontReader" )
        );
    }
}