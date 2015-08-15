#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        IkConstraintData::IkConstraintData(const char *name)
            : name(name)
            , bone_count(0)
            , bones(nullptr)
            , target(nullptr)
            , bend_direction(1)
            , mix(1)
        {

        }

        IkConstraintData::~IkConstraintData(void)
        {
            delete []bones;
        }
    }
}
