#pragma once

#include "ResourceTypeReader.h"
#include "AnimationInfo.h"

namespace ursine
{
    namespace resources
    {
        class AnimationClipReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            AnimationClipReader(void);

        private:
            std::shared_ptr<graphics::ufmt_loader::AnimInfo> m_animInfo;
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "AnimationClipReader" )
        );
    }
}