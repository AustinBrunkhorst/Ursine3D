#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        EventData::EventData(const char *name)
            : name(name)
            , int_value(0)
            , float_value(0)
            , string_value(nullptr)
        {

        }

        EventData::~EventData(void)
        {

        }
    }
}
