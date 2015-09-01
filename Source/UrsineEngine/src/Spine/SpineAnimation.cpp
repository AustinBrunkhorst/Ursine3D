#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

#include <limits.h>

namespace ursine
{
    namespace spine
    {
        SpineAnimation::SpineAnimation(const char *name, int timelinesCount)
            : name(name)
            , duration(0)
            , timeline_count(timelinesCount)
            , timelines(new Timeline* [timelinesCount])
        {

        }

        SpineAnimation::~SpineAnimation(void)
        {
            int i;

            for (i = 0; i < timeline_count; ++i)
                delete timelines[i];

            delete []timelines;
        }

        void SpineAnimation::Apply(Skeleton *skeleton, float lastTime, float time, int loop, SpineEvent **events,
            int *eventsCount)
        {
            int i, n = timeline_count;

            if (loop && duration)
            {
                time = fmod(time, duration);
                lastTime = fmod(lastTime, duration);
            }

            for (i = 0; i < n; ++i)
                timelines[i]->Apply(skeleton, lastTime, time, events, eventsCount, 1);
        }

        void SpineAnimation::Mix(Skeleton *skeleton, float lastTime, float time, int loop, SpineEvent **events,
            int *eventsCount, float alpha)
        {
            int i, n = timeline_count;

            if (loop && duration)
            {
                time = fmod(time, duration);
                lastTime = fmod(lastTime, duration);
            }

            for (i = 0; i < n; ++i)
                timelines[i]->Apply(skeleton, lastTime, time, events, eventsCount, alpha);
        }

        const char *SpineAnimation::GetName(void) const
        {
            return name.c_str();
        }

        float SpineAnimation::GetDuration(void) const
        {
            return duration;
        }
    }
}
