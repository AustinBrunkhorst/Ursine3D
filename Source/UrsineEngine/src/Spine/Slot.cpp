#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        Slot::Slot(SlotData *Data, Bone *Bone)
            : data(Data)
            , bone(Bone)
            , r(0)
            , g(0)
            , b(0)
            , a(0)
            , attachment(nullptr)
            , attachment_verts_capacity(0)
            , attachment_verts_count(0)
            , attachment_verts(nullptr)
        {
            SetToSetupPose();
        }

        Slot::~Slot(void)
        {
            delete []attachment_verts;
        }

        void Slot::SetAttachment(Attachment *attachment)
        {
            (*(Attachment **)&(this->attachment)) = attachment;

            attachment_time = bone->GetSkeleton()->GetTime();

            attachment_verts_count = 0;
        }

        void Slot::SetAttachmentTime(float time)
        {
            attachment_time = bone->GetSkeleton()->GetTime() - time;
        }

        float Slot::GetAttachmentTime(void) const
        {
            return bone->GetSkeleton()->GetTime() - attachment_time;
        }

        void Slot::SetToSetupPose(void)
        {
            Attachment *attachment = nullptr;

            r = data->r;
            g = data->g;
            b = data->b;
            a = data->a;

            if (data->attachment_name.c_str())
            {
                /* Find slot index. */
                int i;

                for (i = 0; i < bone->GetSkeleton()->GetData()->slot_count; ++i)
                {
                    if (data == bone->GetSkeleton()->GetData()->slots[i])
                    {
                        attachment = bone->GetSkeleton()->GetAttachmentForSlotIndex(i, data->attachment_name.c_str());

                        break;
                    }
                }
            }

            SetAttachment(attachment);
        }
    }
}
