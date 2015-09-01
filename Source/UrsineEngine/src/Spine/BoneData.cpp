#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

namespace ursine
{
    namespace spine
    {
        BoneData::BoneData(const char *name, BoneData *parent)
            : name(name)
            , parent(parent)
            , length(0)
            , x(0)
            , y(0)
            , rotation(0)
            , scale_x(1)
            , scale_y(1)
            , flip_x(0)
            , flip_y(0)
            , inherit_scale(1)
            , inherit_rotation(1)
        {

        }

        BoneData::~BoneData(void)
        {

        }
    }
}
