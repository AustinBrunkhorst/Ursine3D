#include "UrsinePrecompiled.h"

#include "AnimationClipData.h"
#include "AnimationClipReader.h"
#include <Core/CoreSystem.h>
#include "GfxAPI.h"

namespace ursine
{
    namespace resources
    {
        AnimationClipData::AnimationClipData(graphics::ufmt_loader::AnimInfo *animeInfo) 
        {
            // @@@TODO: give model info to graphics system, assign m_modelHandle
            m_animeHandle = GetCoreSystem(graphics::GfxAPI)->ResourceMgr.CreateAnimation( animeInfo );
        }

        AnimationClipData::~AnimationClipData(void) 
        {
            GetCoreSystem(graphics::GfxAPI)->ResourceMgr.DestroyAnimation( m_animeHandle );
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
            graphics::ufmt_loader::AnimInfo *animeInfo = GetCoreSystem(graphics::GfxAPI)->ResourceMgr.GetAnimInfo( m_animeHandle );
            animeInfo->Write( output );
        }

        meta::Type AnimationClipData::GetReaderType(void)
        {
            return typeof( AnimationClipReader );
        }
    }
}