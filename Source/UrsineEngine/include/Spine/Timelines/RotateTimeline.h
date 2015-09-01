#pragma once

#include "Spine/Timelines/BaseTimeline.h"

namespace ursine
{
    namespace spine
    {
        class RotateTimeline : public BaseTimeline
        {
        public:
            RotateTimeline(int frame_count);
            ~RotateTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, float angle);
        };
    }
}
