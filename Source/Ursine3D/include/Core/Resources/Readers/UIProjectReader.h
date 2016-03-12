#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class UIProjectReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            UIProjectReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "UIProjectReader" )
        );
    }
}