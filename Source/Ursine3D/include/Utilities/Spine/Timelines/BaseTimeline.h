#pragma once

#include "Spine/Timelines/CurveTimeline.h"

namespace ursine
{
    namespace spine
    {
        class BaseTimeline : public CurveTimeline
        {
            friend class SkeletonJson;

        protected:
            const int frame_count;
            std::vector<float> frames; /* time, angle, ... for rotate. time, x, y, ... for translate and scale. */
            int bone_index;

        public:
            BaseTimeline(TimelineType type, int frame_count, int frame_size);
            ~BaseTimeline(void);
        };
    }
}
