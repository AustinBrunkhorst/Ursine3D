#pragma once

#include "Spine/Timeline.h"

namespace ursine
{
    namespace spine
    {
        class AttachmentTimeline : Timeline
        {
            friend class SkeletonJson;

            int frame_count;
            std::vector<float> frames; /* time, ... */

            int slot_index;
            std::vector<std::string> attachment_names;

        public:
            AttachmentTimeline(int frame_count);
            ~AttachmentTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, const char *attachmentName);
        };
    }
}
