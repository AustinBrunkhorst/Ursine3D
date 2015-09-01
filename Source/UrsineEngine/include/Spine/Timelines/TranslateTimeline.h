#pragma once

#include "Spine/Timelines/BaseTimeline.h"

namespace ursine
{
    namespace spine
    {
        class TranslateTimeline : public BaseTimeline
        {
        public:
            TranslateTimeline(int frame_count);
            ~TranslateTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, float x, float y);
        };
    }
}
