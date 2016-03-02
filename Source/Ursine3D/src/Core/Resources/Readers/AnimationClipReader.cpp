#include "UrsinePrecompiled.h"

#include "AnimationClipReader.h"
#include "AnimationClipData.h"

namespace ursine
{
    namespace resources
    {
        AnimationClipReader::AnimationClipReader(void)
            : m_animInfo( std::make_shared<graphics::ufmt_loader::AnimInfo>( ) )
        { 
        }

        ResourceData::Handle AnimationClipReader::Read(ResourceReader &input)
        {
            m_animInfo->Read(input);
            return std::make_shared<AnimationClipData>(m_animInfo.get());
        }
    }
}