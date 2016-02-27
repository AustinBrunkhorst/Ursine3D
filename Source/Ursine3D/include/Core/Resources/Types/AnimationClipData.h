#pragma once

#include "ResourceData.h"

#include "GfxDefines.h"
#include "AnimationInfo.h"

namespace ursine
{
    namespace resources
    {
        class AnimationClipData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            AnimationClipData();
            AnimationClipData( graphics::ufmt_loader::AnimInfo *animeInfo );
            ~AnimationClipData( void );

            graphics::GfxHND GetAnimeHandle( void ) const;
            void Writing( pipeline::ResourceWriter &output );

        private:
            graphics::GfxHND m_animeHandle;

            void Write( pipeline::ResourceWriter &output ) override;

            meta::Type GetReaderType(void) override;
        } Meta( Register );
    }
}