#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        SlotData::SlotData(const char *name, BoneData *bone_data)
            : name(name)
            , bone_data(bone_data)
            , r(1)
            , g(1)
            , b(1)
            , a(1)
            , additive_blending(0)
        {

        }

        SlotData::~SlotData(void)
        {

        }

        void SlotData::SetAttachmentName(const char *attachment_name)
        {
            if (attachment_name)
                this->attachment_name = attachment_name;
            else
                this->attachment_name.clear();
        }
    }
}
