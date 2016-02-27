#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class WorldReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            WorldReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "WorldReader" )
        );
    }
}