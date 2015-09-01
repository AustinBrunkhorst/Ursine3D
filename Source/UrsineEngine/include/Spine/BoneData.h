#pragma once

namespace ursine
{
    namespace spine
    {
        struct BoneData
        {
            const std::string name;

            BoneData * const parent;

            float length;

            float x;
            float y;

            float rotation;
            float scale_x;
            float scale_y;

            bool flip_x;
            bool flip_y;

            bool inherit_scale, inherit_rotation;

            BoneData(const char *name, BoneData *parent);
            ~BoneData(void);
        };
    }
}
