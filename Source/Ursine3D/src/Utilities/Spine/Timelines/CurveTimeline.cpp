#include "UrsinePrecompiled.h"

#include "Spine/Timelines/CurveTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace ursine
{
    namespace spine
    {
        CurveTimeline::CurveTimeline(TimelineType type, int frame_count)
            : Timeline(type)
            , curves((frame_count - 1) * BEZIER_SIZE, 0.0f)
        {

        }

        CurveTimeline::~CurveTimeline(void)
        {

        }

        void CurveTimeline::SetLinear(int frameIndex)
        {
            curves[frameIndex * BEZIER_SIZE] = CURVE_LINEAR;
        }

        void CurveTimeline::SetStepped(int frameIndex)
        {
            curves[frameIndex * BEZIER_SIZE] = CURVE_STEPPED;
        }

        void CurveTimeline::SetCurve(int frameIndex, float cx1, float cy1, float cx2, float cy2)
        {
            float subdiv1 = 1.0f / BEZIER_SEGMENTS, subdiv2 = subdiv1 * subdiv1, subdiv3 = subdiv2 * subdiv1;
            float pre1 = 3 * subdiv1, pre2 = 3 * subdiv2, pre4 = 6 * subdiv2, pre5 = 6 * subdiv3;
            float tmp1x = -cx1 * 2 + cx2, tmp1y = -cy1 * 2 + cy2, tmp2x = (cx1 - cx2) * 3 + 1, tmp2y = (cy1 - cy2) * 3 + 1;
            float dfx = cx1 * pre1 + tmp1x * pre2 + tmp2x * subdiv3, dfy = cy1 * pre1 + tmp1y * pre2 + tmp2y * subdiv3;
            float ddfx = tmp1x * pre4 + tmp2x * pre5, ddfy = tmp1y * pre4 + tmp2y * pre5;
            float dddfx = tmp2x * pre5, dddfy = tmp2y * pre5;
            float x = dfx, y = dfy;

            int i = frameIndex * BEZIER_SIZE, n = i + BEZIER_SIZE - 1;

            curves[i++] = CURVE_BEZIER;

            for (; i < n; i += 2)
            {
                curves[i] = x;
                curves[i + 1] = y;
                dfx += ddfx;
                dfy += ddfy;
                ddfx += dddfx;
                ddfy += dddfy;
                x += dfx;
                y += dfy;
            }
        }

        float CurveTimeline::GetCurvePercent(int frameIndex, float percent)
        {
            float x, y;

            int i = frameIndex * BEZIER_SIZE, start, n;

            float type = curves[i];

            if (type == CURVE_LINEAR)
                return percent;

            if (type == CURVE_STEPPED)
                return 0;

            i++;

            x = 0;

            for (start = i, n = i + BEZIER_SIZE - 1; i < n; i += 2)
            {
                x = curves[i];

                if (x >= percent)
                {
                    float prevX, prevY;

                    if (i == start)
                    {
                        prevX = 0;
                        prevY = 0;
                    }
                    else
                    {
                        prevX = curves[i - 2];
                        prevY = curves[i - 1];
                    }

                    return prevY + (curves[i + 1] - prevY) * (percent - prevX) / (x - prevX);
                }
            }

            y = curves[i - 1];

            return y + (1 - y) * (percent - x) / (1 - x); /* Last point is 1,1. */
        }
    }
}
