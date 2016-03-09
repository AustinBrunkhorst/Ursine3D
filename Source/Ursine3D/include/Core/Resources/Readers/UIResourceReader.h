#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class UIResourceReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            UIResourceReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "UIResourceReader" )
        );
    }
}