#pragma once

#include "Spine/Timelines/CurveTimeline.h"

namespace Ursine
{
    namespace Spine
    {
        class FFDTimeline : public CurveTimeline
        {
            friend class SkeletonJson;

            int const frame_count;
            std::vector<float> frames; /* time, ... */
            int const frame_vertex_count;
            std::vector<std::vector<float>> frame_vertices;
            int slot_index;
            Attachment *attachment;

        public:
            FFDTimeline(int framesCount, int frameVerticesCount);
            ~FFDTimeline(void);

            void Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents,
                int *eventsCount, float alpha) override;

            void SetFrame(int frameIndex, float time, float *vertices);
        };
    }
}
