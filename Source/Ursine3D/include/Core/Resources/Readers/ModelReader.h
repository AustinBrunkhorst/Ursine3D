#pragma once

#include "ResourceTypeReader.h"
#include "ModelInfo.h"

namespace ursine
{
    namespace resources
    {
        class ModelReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            ModelReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "ModelReader" )
        );
    }
}