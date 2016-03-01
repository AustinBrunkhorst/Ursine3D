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
            std::shared_ptr<graphics::ufmt_loader::ModelInfo> m_modelInfo;
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "ModelReader" )
        );
    }
}