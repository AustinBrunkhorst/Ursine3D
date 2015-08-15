#include "UrsinePrecompiled.h"

#include "Spine/Timelines/ColorTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace Ursine
{
    namespace Spine
    {
        ColorTimeline::ColorTimeline(int frames_count)
            : BaseTimeline(SP_TIMELINE_COLOR, frames_count, 5)
            , slot_index(0)
        {

        }

        ColorTimeline::~ColorTimeline(void) {}

        void ColorTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            Slot *slot;
            int frameIndex;
            float prevFrameR, prevFrameG, prevFrameB, prevFrameA, percent, frameTime;
            float r, g, b, a;

            if (time < frames[0])
                return; /* Time is before first frame. */

            if (time >= this->frames[this->frame_count - 5])
            {
                /* Time is after last frame. */
                int i = this->frame_count - 1;

                r = frames[i - 3];
                g = frames[i - 2];
                b = frames[i - 1];
                a = frames[i];
            }
            else
            {
                /* Interpolate between the previous frame and the current frame. */
                frameIndex = BinarySearch(&frames[0], frame_count, time, 5);

                prevFrameR = frames[frameIndex - 4];
                prevFrameG = frames[frameIndex - 3];
                prevFrameB = frames[frameIndex - 2];
                prevFrameA = frames[frameIndex - 1];
                frameTime = frames[frameIndex];

                percent = 1 - (time - frameTime) / (frames[frameIndex + COLOR_PREV_FRAME_TIME] - frameTime);

                percent = GetCurvePercent(frameIndex / 5 - 1, percent < 0 ? 0 : (percent > 1 ? 1 : percent));

                r = prevFrameR + (frames[frameIndex + COLOR_FRAME_R] - prevFrameR) * percent;
                g = prevFrameG + (frames[frameIndex + COLOR_FRAME_G] - prevFrameG) * percent;
                b = prevFrameB + (frames[frameIndex + COLOR_FRAME_B] - prevFrameB) * percent;
                a = prevFrameA + (frames[frameIndex + COLOR_FRAME_A] - prevFrameA) * percent;
            }

            slot = skeleton->GetSlots()[slot_index];

            if (alpha < 1)
            {
                slot->r += (r - slot->r) * alpha;
                slot->g += (g - slot->g) * alpha;
                slot->b += (b - slot->b) * alpha;
                slot->a += (a - slot->a) * alpha;
            }
            else
            {
                slot->r = r;
                slot->g = g;
                slot->b = b;
                slot->a = a;
            }
        }

        void ColorTimeline::SetFrame(int frame_index, float time, float r, float g, float b, float a)
        {
            frame_index *= 5;

            this->frames[frame_index] = time;
            this->frames[frame_index + 1] = r;
            this->frames[frame_index + 2] = g;
            this->frames[frame_index + 3] = b;
            this->frames[frame_index + 4] = a;
        }
    }
}
