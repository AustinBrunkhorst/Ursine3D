#include "UrsinePrecompiled.h"

#include "Spine/Timelines/BaseTimeline.h"

namespace Ursine
{
    namespace Spine
    {
        BaseTimeline::BaseTimeline(TimelineType type, int frame_count, int frame_size)
            : CurveTimeline(type, frame_count)
            , frame_count(frame_count * frame_size)
            , frames(frame_count * frame_size)
            , bone_index(0)
        {

        }

        BaseTimeline::~BaseTimeline(void)
        {

        }
    }
}