#include "UrsinePrecompiled.h"

#include "Spine/Timelines/FlipTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace ursine
{
    namespace spine
    {
        FlipTimeline::FlipTimeline(int framesCount, bool x)
            : Timeline(x ? SP_TIMELINE_FLIPX : SP_TIMELINE_FLIPY)
            , x(x)
            , frame_count(framesCount << 1)
            , frames(frame_count, 0.0f)
            , bone_index(0)
        {

        }

        FlipTimeline::~FlipTimeline(void)
        {

        }

        void FlipTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            int frameIndex;

            if (time < frames[0])
            {
                if (lastTime > time)
                    Apply(skeleton, lastTime, (float)INT_MAX, 0, 0, 0);

                return;
            }
            else if (lastTime > time) /**/
                lastTime = -1;

            frameIndex = (time >= frames[frame_count - 2] ?
                frame_count : BinarySearch(&frames[0], frame_count, time, 2)) - 2;

            if (frames[frameIndex] < lastTime)
                return;

            if (x)
                skeleton->GetBones()[bone_index]->SetFlipX((int)frames[frameIndex + 1]);
            else
                skeleton->GetBones()[bone_index]->SetFlipY((int)frames[frameIndex + 1]);
        }

        void FlipTimeline::SetFrame(int frameIndex, float time, bool flip)
        {
            frameIndex <<= 1;

            frames[frameIndex] = time;
            frames[frameIndex + 1] = (float)flip;
        }
    }
}
