#pragma once

#include "Spine/Timelines/BaseTimeline.h"

namespace Ursine
{
    namespace Spine
    {
        class IkConstraintTimeline : public BaseTimeline
        {
            friend class SkeletonJson;

            int constraint_index;

        public:
            IkConstraintTimeline(int framesCount);
            ~IkConstraintTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, float mix, int bendDirection);
        };
    }
}
