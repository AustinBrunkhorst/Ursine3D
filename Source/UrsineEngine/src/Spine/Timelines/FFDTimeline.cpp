#include "UrsinePrecompiled.h"

#include "Spine/Timelines/FFDTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace Ursine
{
    namespace Spine
    {
        FFDTimeline::FFDTimeline(int framesCount, int frameVerticesCount)
            : CurveTimeline(SP_TIMELINE_FFD, framesCount)
            , frame_count(framesCount)
            , frames(framesCount, 0.0f)
            , frame_vertex_count(frameVerticesCount)
            , frame_vertices(framesCount)
            , slot_index(0)
        {

        }

        FFDTimeline::~FFDTimeline(void)
        {

        }

        void FFDTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            int frameIndex, i;
            float percent, frameTime;
            const float *prevVertices;
            const float *nextVertices;

            Slot *slot = skeleton->GetSlots()[slot_index];

            if (slot->attachment != attachment)
                return;

            if (time < frames[0])
                return; /* Time is before first frame. */

            if (slot->attachment_verts_count < frame_vertex_count)
            {
                if (slot->attachment_verts_capacity < frame_vertex_count)
                {
                    delete []slot->attachment_verts;
                    slot->attachment_verts = new float[frame_vertex_count];
                    slot->attachment_verts_capacity = frame_vertex_count;
                }
            }

            if (slot->attachment_verts_count != frame_vertex_count)
                alpha = 1; /* Don't mix from uninitialized slot vertices. */

            slot->attachment_verts_count = frame_vertex_count;

            if (time >= frames[frame_count - 1])
            {
                /* Time is after last frame. */
                const float *lastVertices = &frame_vertices[frame_count - 1][0];
                if (alpha < 1)
                {
                    for (i = 0; i < frame_vertex_count; ++i)
                        slot->attachment_verts[i] += (lastVertices[i] - slot->attachment_verts[i]) * alpha;
                }
                else
                    memcpy(slot->attachment_verts, lastVertices, frame_vertex_count * sizeof(float));
                return;
            }

            /* Interpolate between the previous frame and the current frame. */
            frameIndex = BinarySearch(&frames[0], frame_count, time);
            frameTime = frames[frameIndex];
            percent = 1 - (time - frameTime) / (frames[frameIndex - 1] - frameTime);
            percent = GetCurvePercent(frameIndex - 1, percent < 0 ? 0 : (percent > 1 ? 1 : percent));

            prevVertices = &frame_vertices[frameIndex - 1][0];
            nextVertices = &frame_vertices[frameIndex][0];

            if (alpha < 1)
            {
                for (i = 0; i < frame_vertex_count; ++i)
                {
                    float prev = prevVertices[i];
                    slot->attachment_verts[i] += (prev + (nextVertices[i] - prev) * percent - slot->attachment_verts[i]) * alpha;
                }
            }
            else
            {
                for (i = 0; i < frame_vertex_count; ++i)
                {
                    float prev = prevVertices[i];
                    slot->attachment_verts[i] = prev + (nextVertices[i] - prev) * percent;
                }
            }
        }

        void FFDTimeline::SetFrame(int frameIndex, float time, float *vertices)
        {
            this->frames[frameIndex] = time;

            

            if (!vertices)
                frame_vertices[frameIndex].clear();
            else
            {
                frame_vertices[frameIndex].resize(frame_vertex_count);

                memcpy(&frame_vertices[frameIndex][0], vertices, frame_vertex_count * sizeof(float));
            }
        }
    }
}
