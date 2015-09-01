#pragma once

#include "Spine/Timeline.h"

namespace ursine
{
    namespace spine
    {
        class CurveTimeline : public Timeline
        {
            std::vector<float> curves; /* type, x, y, ... */

        public:
            CurveTimeline(TimelineType type, int frame_count);
            ~CurveTimeline(void);

            void SetLinear(int frameIndex);
            void SetStepped(int frameIndex);

            void SetCurve(int frameIndex, float cx1, float cy1, float cx2, float cy2);
            float GetCurvePercent(int frameIndex, float percent);
        };
    }
}
