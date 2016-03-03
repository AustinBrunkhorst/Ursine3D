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
            m_animeHandle = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateAnimation( animeInfo );
        }

        AnimationClipData::~AnimationClipData(void) 
        {
            GetCoreSystem( graphics::GfxAPI )->ResourceMgr.DestroyAnimation( m_animeHandle );
        }

        graphics::GfxHND AnimationClipData::GetAnimeHandle(void) const
        {
            return m_animeHandle;
        }

        void AnimationClipData::Writing(pipeline::ResourceWriter &output)
        {
            Write(output);
        }

        void AnimationClipData::Write(pipeline::ResourceWriter &output)
        {
            auto animeInfo = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.GetAnimInfo( m_animeHandle );

            animeInfo.Write( output );
        }

        meta::Type AnimationClipData::GetReaderType(void)
        {
            return typeof( AnimationClipReader );
        }
    }
}