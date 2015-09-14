#pragma once

#include "Spine/Bone.h"
#include "Spine/Attachment.h"
#include "Spine/SlotData.h"

namespace ursine
{
    namespace spine
    {
        struct Slot
        {
            float attachment_time;

            SlotData * const data;
            Bone * const bone;

            float r;
            float g;
            float b;
            float a;

            Attachment * const attachment;

            int attachment_verts_capacity;
            int attachment_verts_count;
            float *attachment_verts;

            Slot(SlotData *data, Bone *bone);
            ~Slot(void);

            void SetAttachment(Attachment *attachment);

            void SetAttachmentTime(float time);
            float GetAttachmentTime(void) const;

            void SetToSetupPose(void);
        };
    }
}
