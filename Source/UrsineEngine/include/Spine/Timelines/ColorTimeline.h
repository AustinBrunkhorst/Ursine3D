#pragma once

#include "Spine/Timelines/BaseTimeline.h"

namespace Ursine
{
    namespace Spine
    {
        class ColorTimeline : BaseTimeline
        {
            friend class SkeletonJson;

            int slot_index;

        public:
            ColorTimeline(int frames_count);
            ~ColorTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, float r, float g, float b, float a);
        };
    }
}
