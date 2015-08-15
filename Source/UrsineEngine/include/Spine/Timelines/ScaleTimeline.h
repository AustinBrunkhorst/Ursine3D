#pragma once

#include "Spine/Timelines/BaseTimeline.h"

namespace Ursine
{
    namespace Spine
    {
        class ScaleTimeline : public BaseTimeline
        {
        public:
            ScaleTimeline(int frame_count);
            ~ScaleTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, float x, float y);
        };
    }
}
