#include "UrsinePrecompiled.h"

#include "AnimationClipReader.h"
#include "AnimationClipData.h"

namespace ursine
{
    namespace resources
    {
        AnimationClipReader::AnimationClipReader(void)
        { 
        }

        ResourceData::Handle AnimationClipReader::Read(ResourceReader &input)
        {
            graphics::ufmt_loader::AnimInfo animInfo;
            
            animInfo.Read(input);

            return std::make_shared<AnimationClipData>( animInfo );
        }
    }
}