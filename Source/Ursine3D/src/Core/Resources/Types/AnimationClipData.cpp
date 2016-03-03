#include "UrsinePrecompiled.h"

#include "AnimationClipData.h"
#include "AnimationClipReader.h"
#include <Core/CoreSystem.h>
#include "GfxAPI.h"

namespace ursine
{
    namespace resources
    {
        AnimationClipData::AnimationClipData(const graphics::ufmt_loader::AnimInfo &animeInfo) 
        {
            m_animHandle = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateAnimation( animeInfo );
        }

        AnimationClipData::~AnimationClipData(void) 
        {
            GetCoreSystem( graphics::GfxAPI )->ResourceMgr.DestroyAnimation( m_animHandle );
        }

        graphics::GfxHND AnimationClipData::GetAnimeHandle(void) const
        {
            return m_animHandle;
        }

        void AnimationClipData::Write(pipeline::ResourceWriter &output)
        {
            auto *animInfo = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.GetAnimInfo( m_animHandle );

            animInfo->Write( output );
        }

        meta::Type AnimationClipData::GetReaderType(void)
        {
            return typeof( AnimationClipReader );
        }
    }
}