#pragma once

#include "Spine/BoneData.h"

namespace ursine
{
    namespace spine
    {
        struct IkConstraintData
        {
            const std::string name;

            int bone_count;
            BoneData **bones;

            BoneData *target;
            int bend_direction;
            float mix;

            IkConstraintData(const char *name);
            ~IkConstraintData(void);
        };
    }
}
