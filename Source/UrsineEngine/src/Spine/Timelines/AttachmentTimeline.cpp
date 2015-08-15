#include "UrsinePrecompiled.h"

#include "Spine/Timelines/AttachmentTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace Ursine
{
    namespace Spine
    {
        AttachmentTimeline::AttachmentTimeline(int frame_count)
            : Timeline(SP_TIMELINE_ATTACHMENT)
            , frame_count(frame_count)
            , frames(frame_count, 0.0f)
            , attachment_names(frame_count)
        {
            URSINE_TODO("test this");
        }

        AttachmentTimeline::~AttachmentTimeline(void)
        {

        }

        void AttachmentTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            int frameIndex;
            const char *attachmentName;

            if (time < frames[0])
            {
                if (lastTime > time)
                    Apply(skeleton, lastTime, (float)INT_MAX, 0, 0, 0);

                return;
            }

            if (lastTime > time) /**/
                lastTime = -1;

            frameIndex = time >= frames[frame_count - 1] ?
                frame_count - 1 : BinarySearch(&frames[0], frame_count, time) - 1;

            if (frames[frameIndex] < lastTime)
                return;

            attachmentName = attachment_names[frameIndex].c_str();

            skeleton->GetSlots()[slot_index]->SetAttachment(
                attachmentName ? skeleton->GetAttachmentForSlotIndex(slot_index, attachmentName) : nullptr);
        }

        void AttachmentTimeline::SetFrame(int frameIndex, float time, const char *attachmentName)
        {
            frames[frameIndex] = time;

            if (attachmentName)
                attachment_names[frameIndex] = attachmentName;
        }
    }
}
