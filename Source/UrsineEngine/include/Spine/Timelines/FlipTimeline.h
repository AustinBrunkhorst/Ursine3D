#pragma once

#include "Spine/Timeline.h"

namespace Ursine
{
    namespace Spine
    {
        class FlipTimeline : Timeline
        {
            friend class SkeletonJson;

            int const x;
            int const frame_count;
            std::vector<float> frames; /* time, flip, ... */
            int bone_index;

        public:
            FlipTimeline(int framesCount, bool x);
            ~FlipTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, bool flip);
        };
    }
}
