#pragma once

#include "Spine/Event.h"
#include "Spine/Skeleton.h"

namespace Ursine
{
    namespace Spine
    {
        enum TimelineType
        {
            SP_TIMELINE_SCALE,
            SP_TIMELINE_ROTATE,
            SP_TIMELINE_TRANSLATE,
            SP_TIMELINE_COLOR,
            SP_TIMELINE_ATTACHMENT,
            SP_TIMELINE_EVENT,
            SP_TIMELINE_DRAWORDER,
            SP_TIMELINE_FFD,
            SP_TIMELINE_IKCONSTRAINT,
            SP_TIMELINE_FLIPX,
            SP_TIMELINE_FLIPY
        };

        class Timeline
        {
        public:
            const TimelineType type;

            Timeline(TimelineType type);
            virtual ~Timeline(void);

            virtual void Apply(
                Skeleton *skeleton,
                float lastTime,
                float time,
                SpineEvent **firedEvents,
                int *eventsCount,
                float alpha) { }
        };
    }
}