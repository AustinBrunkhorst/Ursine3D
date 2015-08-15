#pragma once

#include "Spine/Timeline.h"

namespace Ursine
{
    namespace Spine
    {
        class EventTimeline : public Timeline
        {
            friend class SkeletonJson;

            int const frame_count;
            std::vector<float> frames; /* time, ... */
            std::vector<SpineEvent *> events;

        public:
            EventTimeline(int frame_count);
            ~EventTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, SpineEvent *event);
        };
    }
}
