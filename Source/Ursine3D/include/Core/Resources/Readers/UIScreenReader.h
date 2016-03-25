#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class UIScreenReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            UIScreenReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "UIScreenReader" )
        );
    }
}