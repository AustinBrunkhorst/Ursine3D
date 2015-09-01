#pragma once

#include "Spine/BoneData.h"

namespace ursine
{
    namespace spine
    {
        struct SlotData
        {
            const std::string name;

            const BoneData * const bone_data;

            std::string attachment_name;

            float r;
            float g;
            float b;
            float a;

            bool additive_blending;

            SlotData(const char *name, BoneData *bone_data);
            ~SlotData(void);

            void SetAttachmentName(const char *attachmentName);
        };
    }
}
