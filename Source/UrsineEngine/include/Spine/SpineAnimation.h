#pragma once

#include "Spine/Event.h"
#include "Spine/Attachment.h"

namespace ursine
{
    namespace spine
    {
        class Skeleton;
        class Timeline;

        class SpineAnimation
        {
            friend class SkeletonJson;

            const std::string name;
            float duration;

            int timeline_count;
            Timeline **timelines;

        public:
            SpineAnimation(const char *name, int timelinesCount);
            ~SpineAnimation(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, int loop,
                SpineEvent **events, int *eventsCount);

            void Mix(Skeleton *skeleton, float lastTime, float time, int loop,
                SpineEvent **events, int *eventsCount, float alpha);

            const char *GetName(void) const;

            float GetDuration(void) const;
        };
    }
}
