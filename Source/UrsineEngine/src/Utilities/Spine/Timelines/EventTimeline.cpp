#include "UrsinePrecompiled.h"

#include "Spine/Timelines/EventTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace ursine
{
    namespace spine
    {
        EventTimeline::EventTimeline(int frame_count)
            : Timeline(SP_TIMELINE_EVENT)
            , frame_count(frame_count)
            , frames(frame_count, 0.0f)
            , events(frame_count, nullptr)
        {

        }

        EventTimeline::~EventTimeline(void)
        {
            int i;

            for (i = 0; i < frame_count; ++i)
                delete events[i];
        }

        void EventTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            int frameIndex;

            if (!firedEvents)
                return;

            if (lastTime > time)
            { /* Fire events after last time for looped animations. */
                Apply(skeleton, lastTime, (float)INT_MAX, firedEvents, eventsCount, alpha);

                lastTime = -1;
            }
            else if (lastTime >= frames[frame_count - 1]) /* Last time is after last frame. */
                return;
            if (time < frames[0])
                return; /* Time is before first frame. */

            if (lastTime < frames[0])
                frameIndex = 0;
            else
            {
                float frame;

                frameIndex = BinarySearch(&frames[0], frame_count, lastTime);

                frame = frames[frameIndex];

                while (frameIndex > 0)
                { /* Fire multiple events with the same frame. */
                    if (frames[frameIndex - 1] != frame)
                        break;
                    frameIndex--;
                }
            }

            for (; frameIndex < frame_count && time >= frames[frameIndex]; ++frameIndex)
            {
                firedEvents[*eventsCount] = events[frameIndex];

                (*eventsCount)++;
            }
        }

        void EventTimeline::SetFrame(int frameIndex, float time, SpineEvent *event)
        {
            frames[frameIndex] = time;

            delete events[frameIndex];

            events[frameIndex] = event;
        }
    }
}
