#pragma once

#include "Spine/Timeline.h"

namespace Ursine
{
    namespace Spine
    {
        class DrawOrderTimeline : public Timeline
        {
            friend class SkeletonJson;

            int const frame_count;
            std::vector<float> frames; /* time, ... */
            std::vector<std::vector<int>> draw_orders;
            int const slot_count;

        public:
            DrawOrderTimeline(int framesCount, int slotsCount);
            ~DrawOrderTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, const int *drawOrder);
        };
    }
}
