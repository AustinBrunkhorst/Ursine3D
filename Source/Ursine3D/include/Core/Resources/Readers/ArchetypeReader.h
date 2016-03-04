#pragma once

#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class ArchetypeReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            ArchetypeReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "ArchetypeReader" )
        );
    }
}