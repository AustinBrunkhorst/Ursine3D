#include "UrsinePrecompiled.h"

#include "Spine/Timelines/DrawOrderTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace Ursine
{
    namespace Spine
    {
        DrawOrderTimeline::DrawOrderTimeline(int framesCount, int slotsCount)
            : Timeline(SP_TIMELINE_DRAWORDER)
            , frame_count(framesCount)
            , frames(framesCount, 0.0f)
            , draw_orders(framesCount)
            , slot_count(slotsCount)
        {

        }

        DrawOrderTimeline::~DrawOrderTimeline(void)
        {

        }

        void DrawOrderTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            int i;
            int frameIndex;
            const int *drawOrderToSetupIndex;

            if (time < frames[0])
                return; /* Time is before first frame. */

            if (time >= frames[frame_count - 1]) /* Time is after last frame. */
                frameIndex = frame_count - 1;
            else
                frameIndex = BinarySearch(&frames[0], frame_count, time) - 1;

            drawOrderToSetupIndex = &draw_orders[frameIndex][0];

            if (!drawOrderToSetupIndex)
                memcpy(skeleton->GetDrawOrder(), skeleton->GetSlots(), slot_count * sizeof(Slot*));
            else
            {
                for (i = 0; i < slot_count; ++i)
                    skeleton->GetDrawOrder()[i] = skeleton->GetSlots()[drawOrderToSetupIndex[i]];
            }
        }

        void DrawOrderTimeline::SetFrame(int frameIndex, float time, const int *drawOrder)
        {
            this->frames[frameIndex] = time;

            if (!drawOrder)
                draw_orders[frameIndex].clear();
            else
            {
                draw_orders[frameIndex].resize(slot_count);

                memcpy(&draw_orders[frameIndex][0], drawOrder, this->slot_count * sizeof(int));
            }
        }
    }
}
