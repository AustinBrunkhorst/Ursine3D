#include "UrsinePrecompiled.h"

#include "AnimationClipData.h"
#include "AnimationClipReader.h"

namespace ursine
{
    namespace resources
    {
        AnimationClipData::AnimationClipData(void) { }

        AnimationClipData::~AnimationClipData(void) { }

        void AnimationClipData::Write(pipeline::ResourceWriter &output)
        {

        }

        meta::Type AnimationClipData::GetReaderType(void)
        {
            return typeof( AnimationClipReader );
        }
    }
}